#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkRootSignatureDx12.h"

namespace tkEngine{
    class CPipelineStatesDx12 {
    public:
        
        static void Init();
		static CRootSignatureDx12 m_modelDrawRootSignature;	//モデル描画用のルートシグネチャ。
        static CShaderDx12 m_skinModelVS;		//スキンモデル用の頂点シェーダー。
        static CShaderDx12 m_skinModelPS;		//スキンモデル用の頂点シェーダー。
        static CShaderDx12 m_nonSkinModelVS;	//ノンスキンモデル用の頂点シェーダー。
		static CShaderDx12 m_spriteVS;			//スプライト描画用の頂点シェーダー。
		static CShaderDx12 m_spritePS;			//スプライト描画用のピクセルシェーダー。
        static ComPtr<ID3D12PipelineState> m_skinModelPipeline;		//スキンモデル描画用のパイプライン。
        static ComPtr<ID3D12PipelineState> m_nonSkinModelPipeline;	//ノンスキンモデル用のパイプライン。
		static ComPtr<ID3D12PipelineState> m_spritePipeline;		//スプライト用のパイプライン。
	private:
		/// <summary>
		/// シェーダーを初期化。
		/// </summary>
		static void InitShaders();
		/// <summary>
		/// ルートシグネチャを初期化。
		/// </summary>
		static void InitRootSignature();
		/// <summary>
		/// パイプラインステートの初期化。
		/// </summary>
		static void InitPipelineState();
    };
}
#endif