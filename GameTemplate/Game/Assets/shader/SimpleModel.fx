/*!
 * @brief	プリミティブ用のシェーダー。
 */

static const float PI = 3.14159265358979323846;

static const float subsurface = 0.0f;
static const float specular = 0.5f;
static const float specularTint = 0.0f;
static const float sheen = 0.0f;
static const float sheenTint = 0.5f;
static const float clearcoat = 0.0f;
static const float clearcoatGloss = 1.0f;
static const float anisotropic = 0.5f;

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1.0f-u, 0.0f, 1.0f);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1.0f) return 1.0f/PI;
    float a2 = a*a;
    float t = 1.0f + (a2-1.0f)*NdotH*NdotH;
    return (a2-1.0f) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a*a;
    float t = 1.0f + (a2-1.0f)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
	float t = sqr(HdotX/ax);
    return 1 / ( PI * ax*ay * sqr( t + sqr(HdotY/ay) + NdotH*NdotH ));
}

float smithG_GGX(float Ndotv, float alphaG)
{
    float a = alphaG*alphaG;
    float b = Ndotv*Ndotv;
    float t = Ndotv + sqrt(a + b - a*b);

    return 1.0f/t;
}

float3 mon2lin(float3 x)
{
    return float3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

/*!
 * @brief	BRDF
 *@param[in]	L				光源までのベクトル。
 *@param[in]	V				視線までのベクトル。
 *@param[in]	normal			法線
 *@param[in]	tangentNormal	接法線
 *@param[in]	biNormal		従法線。
 */
float3 BRDF( 
	float3 L, 
	float3 V, 
	float3 normal, 
	float3 tangentNormal, 
	float3 biNormal, 
	float3 baseColor, 
	float roughness,
	float metallic
)
{
    float NdotL = dot(normal,L);

    float NdotV = max(0.5f, dot(normal,V));
    if (NdotL < 0 ) return 0.0f;

    float3 H = normalize(L+V);

    float NdotH = dot(normal,H);
    float LdotH = dot(L,H);
    
    
    float3 Cdlin = mon2lin(baseColor);
    float Cdlum = 0.3f*Cdlin[0] + 0.6f*Cdlin[1]  + 0.1f*Cdlin[2]; // luminance approx.

    float3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : float3(1.0f, 1.0f, 1.0f); // normalize lum. to isolate hue+sat
    float3 Cspec0 = lerp(specular*0.08f*lerp(float3(1.0f, 1.0f, 1.0f), Ctint, specularTint), Cdlin, metallic);
    float3 Csheen = lerp(float3(1.0f, 1.0f, 1.0f), Ctint, sheenTint);

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
    // and lerp in diffuse retro-reflection based on roughness
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5f + 2.0f * LdotH*LdotH * roughness;
    float Fd = lerp(1.0f, Fd90, FL) * lerp(1.0f, Fd90, FV) ;

    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
    // 1.25 scale is used to (roughly) preserve albedo
    // Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdotH*LdotH*roughness;
    float Fss = lerp(1.0f, Fss90, FL) * lerp(1.0f, Fss90, FV);
    float ss = 1.25f * (Fss * (1.0f / (NdotL + NdotV) - 0.5f) + 0.5f);

    // specular
    float aspect = sqrt(1-anisotropic*0.9f);
    float ax = max(.001, sqr(roughness)/aspect);
    float ay = max(.001, sqr(roughness	)*aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, tangentNormal), dot(H, biNormal), ax, ay);
    float FH = SchlickFresnel(LdotH);
    float3 Fs = lerp(Cspec0, float3(1.0f, 1.0f, 1.0f), FH);
    float roughg = sqr(roughness*.5+.5);
    float Gs = smithG_GGX(NdotL, roughg) * smithG_GGX(NdotV, roughg);

    // sheen
    float3 Fsheen = FH * sheen * Csheen;

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = GTR1(NdotH, lerp(0.1f,0.001f,clearcoatGloss));
    float Fr = lerp(0.04, 1.0f, FH);
    float Gr = smithG_GGX(NdotL, 0.25f) * smithG_GGX(NdotV, 0.25f);
    
    float3 lig = ((1.0f/PI) * lerp(Fd, ss, subsurface)*Cdlin + Fsheen)	//ディフューズ。
        * (1.0f-metallic)											//ディフューズ。
        + (Gs*Fs*Ds + 0.25f*clearcoat*Gr*Fr*Dr) * metallic;			//スペキュラ。
    lig = max(0.0f, lig);
    return lig;
}

float SchlickFresnel(float u, float f0, float f90)
{
  return f0 + (f90-f0)*pow(1.0f-u,5.0f);
}

float3 NormalizedDisneyDiffuse(float3 albedo, float3 N, float3 L, float3 V, float roughness)
{
  float3 H = normalize(L+V);
  float dotLH = saturate(dot(L,H));
 
  float dotNL = saturate(dot(N,L));
  float dotNV = saturate(dot(N,V));
  float Fd90 = 0.5f + 2.0f * dotLH * dotLH * roughness;
  
  float FL = SchlickFresnel(1.0f, Fd90, dotNL);
  float FV = SchlickFresnel(1.0f, Fd90, dotNV);
  return (albedo*FL*FV)/PI;
}
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
	float3 toEye = normalize(eyePos - psIn.worldPos);
	float metaric = 0.0f;
	float4 albedoColor = g_albedoMap.Sample(g_sampler, psIn.uv);
	if(hasSpecularMap){
		//スペキュラマップがある。
		metaric = g_specMap.Sample( g_sampler, psIn.uv).a;
	}
	for( int ligNo = 0; ligNo < numDirectionLight; ligNo++ ){
	#if 0
		lig += max( dot( normal, -directionLight[ligNo].direction ), 0.0f ) * directionLight[ligNo].color / PI;
	#else
		float3 baseColor = max( dot( normal, -directionLight[ligNo].direction ), 0.0f ) * directionLight[ligNo].color;
		lig += NormalizedDisneyDiffuse(
			baseColor, 
			normal,
			directionLight[ligNo].direction,
			reflect(directionLight[ligNo].direction, normal),
			0.5f
		) ;
	#endif	
		/*lig += BRDF(
			directionLight[ligNo].direction * -1.0f,
			toEye,
			normal,
			psIn.Tangent,
			psIn.biNormal,
			albedoColor,
			1.0f-metaric,
			metaric
		);*/
	}
	lig += ambientLight;
	
	float4 finalColor = 1.0f;
	finalColor.xyz = albedoColor.xyz * lig;
	return finalColor;
}
