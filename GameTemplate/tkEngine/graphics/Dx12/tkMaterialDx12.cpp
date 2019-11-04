#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMaterialDx12.h"

namespace tkEngine {
	//ルートシグネチャとパイプラインステート周りはカリカリカリ
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enNumDescriptorHeap
	};
	
	void CMaterialDx12::InitTexture(const CTkmFile::SMaterial& tkmMat)
	{
		if (tkmMat.albedoMap != nullptr) {
			m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
		}
		if (tkmMat.normalMap != nullptr) {
			m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
		}
		if (tkmMat.specularMap != nullptr) {
			m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
		}
	}
	void CMaterialDx12::InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat)
	{
		//テクスチャをロード。
		InitTexture(tkmMat);
	
		//定数バッファを作成。
		SMaterialParam matParam;
		matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
		matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
		m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);
	}

	void CMaterialDx12::BeginRender(IRenderContext& rc, int hasSkin)
	{
		//ルートシグネチャとパイプラインステートを設定。
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		
		rcDx12.SetRootSignature(CPipelineStatesDx12::m_modelDrawRootSignature);
		if (hasSkin) {
			rcDx12.SetPipelineState(CPipelineStatesDx12::m_skinModelPipeline);
		}
		else {
			rcDx12.SetPipelineState(CPipelineStatesDx12::m_nonSkinModelPipeline);
		}
	}

}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12