/*!
 * @brief	プリミティブ用のシェーダー。
 */



 //ディレクションライト。
struct SDirectionLight{
	float4	color;			//!<ライトのカラー。
	float3	direction;		//!<ライトの方向。
	unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてのオブジェクトにライトを当てる。
};

//モデル用の定数バッファ
//これを変更したらtkEngine\graphics\Dx12\tkMeshPartsDx12.hの
//SConstantBufferも変更するように。
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
/*!
 * @brief	ライト用の定数バッファ。
 *@details
 * これを変更したらtkLightManagerDx12.hのSLightParamも変更するように。
 */
cbuffer LightCb : register(b1)
{
	float3 eyePos 			: packoffset(c0);		//カメラの位置。
	int numDirectionLight 	: packoffset(c0.w);		//ディレクションライトの数。
	float3 ambientLight		: packoffset(c1);		//アンビエントライト。
	int numPointLight		: packoffset(c1.w);		//ポイントライトの数。
	float4 screenParam		: packoffset(c2);		// スクリーンパラメータ(near, far, screenWidth, screenHeight)
};

cbuffer MaterialParamCb : register(b2){
	int hasNormalMap;			//!<法線マップある？
	int hasSpecularMap;			//!<スペキュラマップある？
};

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;
	float3 Normal   : NORMAL;
	float3 Tangent  : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv 		: TEXCOORD0;
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;
	float3 Normal   	: NORMAL;
	float3 Tangent		: TANGENT;
	float3 biNormal 	: BINORMAL;
	float2 uv 			: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;
};

//アルベドテクスチャ。
Texture2D<float4> g_albedoMap : register(t0);	//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);	//法線マップ
Texture2D<float4> g_specMap : register(t2);		//スペキュラマップ。
//ボーン行列。
StructuredBuffer<float4x4> boneMatrix : register(t3);
//ディレクションライト。
StructuredBuffer<SDirectionLight> directionLight : register(t4);

//サンプラステート。
sampler g_sampler : register(s0);

//スキン行列を計算する。
float4x4 CalcSkinMatrix(SVSIn In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}

//頂点シェーダーのコア関数。
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.Normal = normalize(mul(m, vsIn.Normal));
	psIn.Tangent = normalize(mul(m, vsIn.Tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;

	return psIn;
}
//スキンなしメッシュの頂点シェーダー。
SPSIn VSMainNonSkin(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
 //テクスチャなしプリミティブ描画用の頂点シェーダー
SPSIn VSMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	//法線を計算。
	float3 normal = psIn.Normal;
	if( hasNormalMap){
		//法線マップから法線を引っ張ってくる。		
		float3 binSpaceNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
		binSpaceNormal = (binSpaceNormal * 2.0f)- 1.0f;
		normal = psIn.Tangent * binSpaceNormal.x 
				+ psIn.biNormal * binSpaceNormal.y 
				+ psIn.Normal * binSpaceNormal.z; 
	}else{
		normal = psIn.Normal;
	}
	//ランバート拡散反射
	float3 lig = 0;
	for( int ligNo = 0; ligNo < numDirectionLight; ligNo++ ){
		float t = dot( normal, directionLight[ligNo].direction * -1.0f);
		t = max( t, 0.0f);
		lig += directionLight[ligNo].color * t;
	}
	if(hasSpecularMap){
		//スペキュラマップがある。
		float specMap = g_specMap.Sample( g_sampler, psIn.uv).a;
		
		float3 toEye = normalize(eyePos - psIn.worldPos);
		for( int ligNo = 0; ligNo < numDirectionLight; ligNo++ ){
			float3 rv = reflect(directionLight[ligNo].direction, normal);
			float t = max(0.0f, dot(rv, toEye) );
			t = pow( t, 3.0f) * specMap * 20; //これはどうするべ。
			lig += directionLight[ligNo].color * t;
		}
	}
	lig += ambientLight;
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
