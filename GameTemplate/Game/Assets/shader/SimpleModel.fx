/*!
 * @brief	プリミティブ用のシェーダー。
 */



 //ディレクションライト。
struct SDirectionLight{
	float4	color;			//!<ライトのカラー。
	float3	direction;		//!<ライトの方向。
	unsigned int lightingMaterialIDGroup;	//!<ライトの影響を与える相手を識別するマテリアルIDグループ。0xFFFFFFFFですべてのオブジェクトにライトを当てる。
};

cbuffer CB : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
/*!
 * @brief	ライト用の定数バッファ。
 *@details
 * これを変更したらtkLightManagerDx12.hのSLightParamも変更するように。
 */
cbuffer lightCb : register(b1)
{
	float3 eyePos 			: packoffset(c0);		//カメラの位置。
	int numDirectionLight 	: packoffset(c0.w);		//ディレクションライトの数。
	float3 ambientLight		: packoffset(c1);		//アンビエントライト。
	int numPointLight		: packoffset(c1.w);		//ポイントライトの数。
	float4 screenParam		: packoffset(c2);		// スクリーンパラメータ(near, far, screenWidth, screenHeight)
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;
	float3 Normal   : NORMAL;
	float2 uv 		: TEXCOORD0;
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 		: SV_POSITION;
	float3 Normal   : NORMAL;
	float2 uv 		: TEXCOORD0;
};

//アルベドテクスチャ。
Texture2D<float4> g_albedo : register(t0);
//ボーン行列。
StructuredBuffer<float4x4> boneMatrix : register(t1);
//ディレクションライト。
StructuredBuffer<SDirectionLight> directionLight : register(t2);

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
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.Normal = vsIn.Normal;
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
	//return float4( psIn.Normal, 1.0f);
	//ランバート拡散反射
	float3 lig = 0;
	for( int ligNo = 0; ligNo < numDirectionLight; ligNo++ ){
		float t = dot( psIn.Normal, directionLight[ligNo].direction * -1.0f);
		t = max( t, 0.0f);
		lig += directionLight[ligNo].color * t;
	}
	lig += ambientLight;
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
