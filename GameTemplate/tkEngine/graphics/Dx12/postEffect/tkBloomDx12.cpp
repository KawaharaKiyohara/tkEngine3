#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkBloomDx12.h"

namespace tkEngine {
	void CBloomDx12::Init(const SGraphicsConfig& config)
	{
		if (!config.bloomConfig.isEnable) {
			return;
		}

		//シェーダーをロード。
		m_vs.Load(L"shader/bloom.fx", "VSMain", g_vsShaderModelName);
		m_psLuminance.Load(L"shader/bloom.fx", "PSSamplingLuminance", g_psShaderModelName);
		m_vsXBlur.Load(L"shader/bloom.fx", "VSXBlur", g_vsShaderModelName);
		m_vsYBlur.Load(L"shader/bloom.fx", "VSYBlur", g_vsShaderModelName);
		m_psBlur.Load(L"shader/bloom.fx", "PSBlur", g_psShaderModelName);
		m_psCombine.Load(L"shader/bloom.fx", "PSCombine", g_psShaderModelName);
		m_copyVS.Load(L"shader/copy.fx", "VSMain", g_vsShaderModelName);
		m_copyPS.Load(L"shader/copy.fx", "PSMain", g_psShaderModelName);

		int w = g_graphicsEngine->GetFrameBufferWidth();
		int h = g_graphicsEngine->GetFrameBufferHeight();
		//輝度抽出用のレンダリングターゲットを作成。
		bool result = m_luminanceRT.Create(w, h, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
		TK_ASSERT(result, "輝度抽出用のレンダリングターゲットの作成に失敗しました。");

		//ダウンサンプリング用のレンダリングターゲットを作成する。
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
			int shift = i + 1;
			int baseIndex = i * 2;
			//横ブラー用。
			result = m_downSamplingRT[baseIndex].Create(
				w >> shift,
				h >> (shift - 1),
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN);

			TK_ASSERT(result, "横ブラー用のレンダリングターゲットの作成に失敗しました。");

			//縦ブラー用。
			result = m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN);
			TK_ASSERT(result, "縦ブラー用のレンダリングターゲットの作成に失敗しました。");


		}

	}
	void CBloomDx12::Render(IRenderContext& rc)
	{

	}
}