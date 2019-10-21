/*!
 * @brief	ライトの管理者。
 */

#include "tkEngine/tkEnginePrecompile.h"
#include "dx12Common.h"
#include "tkEngine/graphics/Dx12/tkLightManagerDx12.h"

namespace tkEngine {
	void CLightManagerDx12::SendLightDataToGPU(IRenderContext& rc)
	{
	}
	void CLightManagerDx12::OnInit()
	{
		//ライトのパラーメータ用の定数バッファを作成。
		m_lightParamCB.Init(sizeof(SLightParam), nullptr);
		//ディレクションライト用のストラクチャバッファを作成。
		m_directionLightSB.Init(sizeof(SDirectionLight), MAX_DIRECTION_LIGHT, nullptr);
	}
	void CLightManagerDx12::OnRender(IRenderContext& rc)
	{
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		m_lightParamCB.Update(&m_lightParam);
		m_directionLightSB.Update(&m_rawDirectionLights);
	}
}