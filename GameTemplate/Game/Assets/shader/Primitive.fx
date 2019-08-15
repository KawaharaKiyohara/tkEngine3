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
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
};

Texture2D<float4> g_texture : register(t0);
sampler g_sampler : register(s0);

 //テクスチャなしプリミティブ描画用の頂点シェーダー
SPSIn VSMainNoTexture( SVSIn vsIn ) 
{
	SPSIn psIn;
	psIn.pos = mul(mWorld, vsIn.pos);
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.color = vsIn.color;
	psIn.uv = vsIn.uv;
	return psIn;
}
//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMainNoTexture( SPSIn psIn ) : SV_Target0
{
	return g_texture.Sample( g_sampler, psIn.uv);
}
