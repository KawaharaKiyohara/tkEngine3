/*!
 * @brief	プリミティブ用のシェーダー。
 */

cbuffer CB : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};


 //テクスチャなしプリミティブ描画用の頂点シェーダー
float4 VSMainNoTexture( float4 pos : SV_Position ) : SV_POSITION
{
	float4 outPos = mul(mWorld, pos);
	outPos = mul(mView, outPos);
	outPos = mul(mProj, outPos);
	return outPos;
}
//テクスチャなしプリミティブ描画用のピクセルシェーダー。
float4 PSMainNoTexture( float4 pos : SV_POSITION ) : SV_Target0
{
	return float4( 1.0f, 0.0f, 0.0f, 1.0f );
}
