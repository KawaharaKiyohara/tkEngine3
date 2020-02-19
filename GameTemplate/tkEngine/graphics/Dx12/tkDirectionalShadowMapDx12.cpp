#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDirectionalShadowMapDx12.h"
#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	void CDirectionalShadowMapDx12::OnRenderToShadowMap(IRenderContext& rc)
	{
		auto& rcDx12 = rc.As<CRenderContextDx12>();

		for (int i = 0; i < NUN_SHADOW_MAP; i++) {
			//�����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂B
			rcDx12.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);
			for (auto& caster : m_shadowCasters) {
				caster->Draw(rc, m_LVPMatrix[i], g_matIdentity);
			}
		}
	}
	void CDirectionalShadowMapDx12::WaitEndRenderToShadowMap(IRenderContext& rc)
	{
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		for (int i = 0; i < NUN_SHADOW_MAP; i++) {
			rcDx12.WaitUntilFinishDrawingToRenderTarget(m_shadowMaps[i]);
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

		//�萔�o�b�t�@���������B
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);
	}
}