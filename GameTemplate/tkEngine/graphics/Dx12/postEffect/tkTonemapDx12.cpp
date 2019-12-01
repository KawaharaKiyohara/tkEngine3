#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkTonemapDx12.h"

namespace tkEngine {
	namespace {

		//-----------------------------------------------------------------------------
		// Name: GetSampleOffsets_DownScale4x4
		// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
		//       pixel shader.
		//-----------------------------------------------------------------------------
		HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, CVector4 avSampleOffsets[])
		{
			if (NULL == avSampleOffsets)
				return E_INVALIDARG;

			float tU = 1.0f / dwWidth;
			float tV = 1.0f / dwHeight;

			// Sample from the 16 surrounding points. Since the center point will be in
			// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
			// center.
			int index = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					avSampleOffsets[index].x = (x - 1.5f) * tU;
					avSampleOffsets[index].y = (y - 1.5f) * tV;

					index++;
				}
			}

			return S_OK;
		}
	}
	void CTonemapDx12::Init(const SGraphicsConfig& config)
	{
		if (!config.tonemapConfig.isEnable) {
			return;
		}
		m_isEnable = true;
		//平均輝度計算用のレンダリングターゲットを作成。
		for (int i = 0; i < NUM_CALC_AVG_RT; i++)
		{
			int rtSize = 1 << (2 * i);
			m_calcAvgRT[i].Create(
				rtSize,
				rtSize,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}
		//最終的にトーンマップで使用する平均輝度を書き込むレンダリングターゲットを作成。
		for (auto& rt : m_avgRT) {
			rt.Create(
				1,
				1,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}
		//ルートシグネチャの初期化。
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		//定数バッファの初期化。
		m_tonemapParam.middleGray = config.tonemapConfig.luminance;
		m_cbTonemapCommon.Init(sizeof(m_tonemapParam) , &m_tonemapParam);
		//シェーダーを初期化。
		m_vs.LoadVS(L"shader/tonemap.fx", "VSMain");
		m_psCalcLuminanceLogAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceLogAvarage");
		m_psCalcLuminanceAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceAvarage");
		m_psCalcLuminanceExpAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceExpAvarage");
		m_psCalcAdaptedLuminanceShader.LoadPS(L"shader/tonemap.fx", "PSCalcAdaptedLuminance");
		m_psCalcAdaptedLuminanceFirstShader.LoadPS(L"shader/tonemap.fx", "PSCalcAdaptedLuminanceFirst");
		m_psFinal.LoadPS(L"shader/tonemap.fx", "PSFinal");

		for (auto& cb : m_cbCalcLuminance) {
			cb.Init(sizeof(m_avSampleOffsets), nullptr);
		}

		//パイプラインステートを初期化。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceLogAvarageShader.GetCompiledBlob().Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
		psoDesc.SampleDesc.Count = 1;
		m_calcLuminanceLogAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceAvarageShader.GetCompiledBlob().Get());
		m_calsLuminanceAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceExpAvarageShader.GetCompiledBlob().Get());
		m_calsLuminanceExpAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminanceFirstShader.GetCompiledBlob().Get());
		m_psCalcAdaptedLuminanceFirstPipelineState.Init(psoDesc);

		psoDesc.PS  = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminanceFirstShader.GetCompiledBlob().Get());
		m_psCalcAdaptedLuminancePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psFinal.GetCompiledBlob().Get());
		m_finalPipelineState.Init(psoDesc);
	}
	void CTonemapDx12::CalcLuminanceAvarage(CRenderContextDx12& rc)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//	
		rc.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
		//対数平均を求める。
		float tU, tV;
		tU = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetWidth());
		tV = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetHeight());

		int index = 0;
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				m_avSampleOffsets[index].x = x * tU;
				m_avSampleOffsets[index].y = y * tV;

				index++;
			}
		}
		int curRtNo = NUM_CALC_AVG_RT - 1;
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);
			m_cbCalcLuminance[curRtNo].Update(m_avSampleOffsets);
			rc.SetPipelineState(m_calcLuminanceLogAvaragePipelineState);
			//シェーダーリソースビューと定数バッファをセットする。
			CConstantBufferDx12* cbrTbl[] = {
				nullptr,
				&m_cbCalcLuminance[curRtNo]
			};
			IShaderResourceDx12* srvTbl[] = {
				&ge12.GetMainRenderTarget().GetRenderTargetTexture()
			};
			rc.SetCBR_SRV_UAV( cbrTbl,srvTbl, 2, 1);

			
			rc.DrawIndexed(4);
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}
		//ここからはダウンサンプリングで求める。
		curRtNo--;
		{
			while (curRtNo > 0) {
				rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
				rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);
				GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
				m_cbCalcLuminance[curRtNo].Update(&m_avSampleOffsets);
				//シェーダーリソースビューと定数バッファをセットする。
				CConstantBufferDx12* cbrTbl[] = {
					nullptr,
					&m_cbCalcLuminance[curRtNo]
				};
				IShaderResourceDx12* srvTbl[] = {
					&m_calcAvgRT[curRtNo + 1].GetRenderTargetTexture()
				};
				rc.SetCBR_SRV_UAV(cbrTbl, srvTbl, 2, 1);
				rc.SetPipelineState(m_calsLuminanceAvaragePipelineState);
				rc.DrawIndexed(4);
				rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
				curRtNo--;
			}
		}
		//最後はexp関数を用いて最終平均を求める。
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);
			GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
			m_cbCalcLuminance[curRtNo].Update(&m_avSampleOffsets);
			//シェーダーリソースビューと定数バッファをセットする。
			CConstantBufferDx12* cbrTbl[] = {
				nullptr,
				&m_cbCalcLuminance[curRtNo]
			};
			IShaderResourceDx12* srvTbl[] = {
				&m_calcAvgRT[curRtNo + 1].GetRenderTargetTexture()
			};
			rc.SetCBR_SRV_UAV( cbrTbl, srvTbl, 2, 1 );
			//パイプラインステートを設定。
			rc.SetPipelineState(m_calsLuminanceExpAvaragePipelineState);
			rc.DrawIndexed(4);
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}

		//明暗順応。
		{
			if (m_isFirstWhenChangeScene == true) {
				//シーンが切り替わって初回。
				m_currentAvgRt = 1 ^ m_currentAvgRt;
				rc.WaitUntilToPossibleSetRenderTarget(m_avgRT[m_currentAvgRt]);
				rc.SetRenderTargetAndViewport(m_avgRT[m_currentAvgRt]);
				IShaderResourceDx12* srvTbl[] = { 
					&m_calcAvgRT[0].GetRenderTargetTexture() 
				};
				
				rc.SetCBR_SRV_UAV( nullptr, srvTbl, 0, 1);
				//パイプラインステートを設定する。
				rc.SetPipelineState(m_psCalcAdaptedLuminanceFirstPipelineState);
				rc.DrawIndexed(4);
				m_isFirstWhenChangeScene = false;
			}
			else {

				auto& lastRT = m_avgRT[m_currentAvgRt];
				m_currentAvgRt = 1 ^ m_currentAvgRt;

				rc.WaitUntilToPossibleSetRenderTarget(m_avgRT[m_currentAvgRt]);
				rc.SetRenderTargetAndViewport(m_avgRT[m_currentAvgRt]);

				IShaderResourceDx12* srvTbl[] = {
					nullptr,
					&m_calcAvgRT[0].GetRenderTargetTexture(),
					&lastRT.GetRenderTargetTexture()
				};
				CConstantBufferDx12* cbrTbl[] = {
					&m_cbTonemapCommon
				};
				rc.SetCBR_SRV_UAV(cbrTbl,srvTbl, 1, 3);
				rc.SetPipelineState(m_psCalcAdaptedLuminancePipelineState);
				rc.DrawIndexed(4);
			}
			rc.WaitUntilFinishDrawingToRenderTarget(m_avgRT[m_currentAvgRt]);
		}
	}
	void CTonemapDx12::Render(IRenderContext& rc)
	{
		if (!m_isEnable) {
			return;
		}
		auto& rc12 = rc.As<CRenderContextDx12>();
		rc12.SetRootSignature(m_rootSignature);
		m_tonemapParam.deltaTime = g_gameTime->GetFrameDeltaTime();
		m_cbTonemapCommon.Update(&m_tonemapParam);
		//平均輝度の計算。
		CalcLuminanceAvarage(rc12);

		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//	
		//平均輝度からトーンマップを行う。
		rc12.WaitUntilToPossibleSetRenderTarget(ge12.GetMainRenderTarget());
		rc12.SetRenderTargetAndViewport(ge12.GetMainRenderTarget());
		CConstantBufferDx12* cbrTbl[] = {
					&m_cbTonemapCommon
		};
		IShaderResourceDx12* srvTbl[] = {
			&ge12.GetMainRenderTarget().GetRenderTargetTexture(),
			& m_avgRT[m_currentAvgRt].GetRenderTargetTexture()
		};
		rc12.SetCBR_SRV_UAV(cbrTbl, srvTbl, 1, 2 );
		rc12.SetPipelineState(m_finalPipelineState);
		rc12.DrawIndexed(4);

		rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
	}
}