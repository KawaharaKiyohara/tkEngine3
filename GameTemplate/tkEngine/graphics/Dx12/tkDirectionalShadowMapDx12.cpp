#include "tkEngine/tkEnginePreCompile.h"
#include "tkDirectionalShadowMapDx12.h"
#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	void CDirectionalShadowMapDx12::RenderToShadowMap(IRenderContext& rc)
	{
		//�����_�����O�X�e�b�v���V���h�E�}�b�v�쐬�ɕύX����B
		rc.SetRenderStep(enRenderStep_CreateDirectionalShadowMap);
		for (int i = 0; i < NUN_SHADOW_MAP; i++) {
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc, m_LVPMatrix[i], g_matIdentity);
			}
		}
	}
}