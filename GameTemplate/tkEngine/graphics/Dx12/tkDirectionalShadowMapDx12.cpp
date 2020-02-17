#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
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
	void CDirectionalShadowMapDx12::OnInit(const SShadowRenderConfig& cfg)
	{
		if (cfg.isEnable == false) {
			return;
		}
		//�V���h�E�}�b�v�̉𑜓x
		int wh[NUN_SHADOW_MAP][2] = {
			{ cfg.shadowMapWidth, cfg.shadowMapHeight},				//�ߋ���
			{ cfg.shadowMapWidth >> 1, cfg.shadowMapHeight >> 1},	//������
			{ cfg.shadowMapWidth >> 1, cfg.shadowMapHeight >> 1},	//������
		};
		int shadowMapNo = 0;
		float clearColor[] = {
			1.0f, 1.0f, 1.0f, 1.0f
		};
		for (auto& sm : m_shadowMaps) {
			sm.Create(
				wh[shadowMapNo][0],
				wh[shadowMapNo][1],
				1,
				1,
				DXGI_FORMAT_R32_FLOAT,
				DXGI_FORMAT_D32_FLOAT,
				clearColor);
		}
	}
}