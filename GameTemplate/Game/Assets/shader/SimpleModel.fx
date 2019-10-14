/*!
 * @brief	プリミティブ用のシェーダー。
 */

cbuffer CB : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	int4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};
//アルベドテクスチャ。
Texture2D<float4> g_albedo : register(t0);
//ボーン行列。
StructuredBuffer<float4x4> boneMatrix : register(t1);
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

 //テクスチャなしプリミティブ描画用の頂点シェーダー
SPSIn VSMainNoTexture( SVSIn vsIn ) 
{
	SPSIn psIn;
	float4x4 m;
	if(vsIn.Indices[0] >= 0){
		//スキンあり。
		m = CalcSkinMatrix(vsIn);
	}else{
		//スキンなし。
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.uv = vsIn.uv;
	return psIn;
}
//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMainNoTexture( SPSIn psIn ) : SV_Target0
{
	float4 texColor = g_albedo.Sample(g_sampler, psIn.uv);
	return float4( texColor.xyz, 1.0f);
}
