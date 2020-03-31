#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDirectionalShadowMapDx12.h"
#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	void CDirectionalShadowMapDx12::OnRenderToShadowMap(IRenderContext& rc)
	{
		auto& rcDx12 = rc.As<CRenderContextDx12>();

		D3D12_RECT scissorRect;
		scissorRect.left = 0;
		scissorRect.top = 0;

		for (int i = 0; i < NUN_SHADOW_MAP; i++) {
			scissorRect.right = m_shadowMaps[i].GetWidth();
			scissorRect.bottom = m_shadowMaps[i].GetHeight();
			rcDx12.SetScissorRect(scissorRect);

			//レンダリングターゲットとして使用可能になるまで待つ。
			rcDx12.WaitUntilToPossibleSetRenderTarget(m_shadowMaps[i]);

			rcDx12.SetRenderTargetAndViewport(m_shadowMaps[i]);
			float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
			rcDx12.ClearRenderTargetView(m_shadowMaps[i], clearColor);
			rcDx12.ClearDepthStencilView(m_shadowMaps[i],1.0);

			for (auto& caster : m_shadowCasters) {
				caster->Draw(rcDx12, m_LVPMatrix[i], g_matIdentity);
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

		//シャドウマップの解像度
		int wh[NUN_SHADOW_MAP][2] = {
			{ cfg.shadowMapWidth, cfg.shadowMapHeight},				//近距離
			{ cfg.shadowMapWidth >> 1, cfg.shadowMapHeight >> 1},	//中距離
			{ cfg.shadowMapWidth >> 1, cfg.shadowMapHeight >> 1},	//遠距離
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

		//定数バッファを初期化。
		m_shadowCb.Init(sizeof(m_shadowCbEntity), nullptr);

	}
}