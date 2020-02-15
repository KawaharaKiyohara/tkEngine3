#include "tkEngine/tkEnginePreCompile.h"
#include "tkDirectionalShadowMapDx12.h"
#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	void CDirectionalShadowMapDx12::RenderToShadowMap(IRenderContext& rc)
	{
		//レンダリングステップをシャドウマップ作成に変更する。
		rc.SetRenderStep(enRenderStep_CreateDirectionalShadowMap);
		for (int i = 0; i < NUN_SHADOW_MAP; i++) {
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc, m_LVPMatrix[i], g_matIdentity);
			}
		}
	}
}