#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMaterialDx12.h"

namespace tkEngine {

	void CMaterialDx12::InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat)
	{
		//todo シェーダーは仮。
		m_vs.Load(L"shader/Primitive.fx", "VSMainNoTexture", "vs_5_0");
		m_ps.Load(L"shader/Primitive.fx", "PSMainNoTexture", "ps_5_0");
		//todo	テクスチャはまだアセットパイプラインができていないので後回し。
		//テクスチャをロード。
		//mat->m_albedoMap.InitFromDDSFile
		//todo 次はパイプラインステートを作成。
		//もう寝る。
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12