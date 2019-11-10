#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkBloomDx12.h"

namespace tkEngine {
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	void CBloomDx12::Init(const SGraphicsConfig& config)
	{
		if (!config.bloomConfig.isEnable) {
			return;
		}

		//シェーダーを初期化。
		InitShaders();
		//レンダリングターゲットを初期化。
		InitRenderTargets();
		//四角形プリミティブを初期化。
		InitQuadPrimitive();
		//パイプラインステートを初期化。
		InitPipelineState();
		//ディスクリプタヒープの初期化。
		InitDescriptorHeap();
		//定数バッファを初期化。
		m_blurParamCB.Init(sizeof(m_blurParam), nullptr);
	}
	void CBloomDx12::InitRenderTargets()
	{
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
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN);

			TK_ASSERT(result, "横ブラー用のレンダリングターゲットの作成に失敗しました。");

			//縦ブラー用。
			result = m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN);
			TK_ASSERT(result, "縦ブラー用のレンダリングターゲットの作成に失敗しました。");

		}
	}
	void CBloomDx12::InitQuadPrimitive()
	{
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		m_quadPrimitive.Init(
			vertices,
			sizeof(vertices),
			sizeof(SSimpleVertex),
			indices,
			sizeof(indices),
			2,
			enPrimitiveTopology_TriangleStrip);
	}
	void CBloomDx12::InitShaders()
	{
		//シェーダーをロード。
		m_vs.Load(L"shader/bloom.fx", "VSMain", g_vsShaderModelName);
		m_psLuminance.Load(L"shader/bloom.fx", "PSSamplingLuminance", g_psShaderModelName);
		m_vsXBlur.Load(L"shader/bloom.fx", "VSXBlur", g_vsShaderModelName);
		m_vsYBlur.Load(L"shader/bloom.fx", "VSYBlur", g_vsShaderModelName);
		m_psBlur.Load(L"shader/bloom.fx", "PSBlur", g_psShaderModelName);
		m_psCombine.Load(L"shader/bloom.fx", "PSCombine", g_psShaderModelName);
		m_copyVS.Load(L"shader/copy.fx", "VSMain", g_vsShaderModelName);
		m_copyPS.Load(L"shader/copy.fx", "PSMain", g_psShaderModelName);

	}
	void CBloomDx12::InitPipelineState()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = CPipelineStatesDx12::m_modelDrawRootSignature.Get(); //モデル描画と共通でも問題ねーわ。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psLuminance.GetCompiledBlob().Get());
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
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_samplingLuminancePipelineState));

		//Xブラー用のパイプラインステート。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsXBlur.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_xblurLuminancePipelineState));

		//yブラー用のパイプラインステート。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsYBlur.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_yblurLuminancePipelineState));

	}
	void CBloomDx12::InitDescriptorHeap()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		//輝度抽出用のディスクリプタヒープを作成。
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_luminanceDescriptorHeap));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");
		
		//ダウンサンプリング用のディスクリプタヒープを作成。
		srvHeapDesc.NumDescriptors = 2;
		for (auto& downSampleDescriptorHeap : m_downSamplingDescriptorHeap) {
			auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&downSampleDescriptorHeap));
			TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");
		}
	}
	void CBloomDx12::UpdateWeight(float dispersion)
	{
		float total = 0;
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] = expf(-0.5f * (float)(i * i) / dispersion);
			total += 2.0f * m_blurParam.weights[i];

		}
		// 規格化
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] /= total;
		}
	}
	void CBloomDx12::SamplingLuminance(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		//メインレンダリングターゲットをテクスチャとして利用するので
		//利用できるようになるまで待機する。
		rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
		rc12.WaitUntilToPossibleSetRenderTarget(m_luminanceRT);
		//パイプラインステートを設定。
		rc12.SetPipelineState(m_samplingLuminancePipelineState);
		//レンダリングターゲットを輝度抽出用に切り替える。
		rc12.SetRenderTarget(m_luminanceRT);
		
		//シェーダーリソースビューと定数バッファをセットする。
		IShaderResourceDx12* srvTbl[] = {
			&ge12.GetMainRenderTarget().GetRenderTargetTexture()
		};
		rc12.SetCBR_SRV_UAV(
			m_luminanceDescriptorHeap.Get(),
			0,
			nullptr,
			1,
			srvTbl
		);
		//ドロドロ。
		rc12.DrawIndexed(4);
		rc12.WaitUntilFinishDrawingToRenderTarget(m_luminanceRT);	
	}
	void CBloomDx12::BlurLuminanceTexture(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		CRenderTargetDx12* prevRt = &m_luminanceRT;

		int rtIndex = 0;
		for (int i = 0; i < /*NUM_DOWN_SAMPLING_RT / 2*/1; i++) {
			//Xブラー。
			{
				m_blurParam.offset.x = 16.0f / prevRt->GetWidth();
				m_blurParam.offset.y = 0.0f;
				m_blurParamCB.Update(&m_blurParam);

				rc12.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				rc12.SetPipelineState(m_xblurLuminancePipelineState);
				D3D12_VIEWPORT viewport;
				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = static_cast<float>(m_downSamplingRT[rtIndex].GetWidth());
				viewport.Height = static_cast<float>(m_downSamplingRT[rtIndex].GetHeight());
				viewport.MinDepth = D3D12_MIN_DEPTH;
				viewport.MaxDepth = D3D12_MAX_DEPTH;
				//rc12.SetViewport(viewport);
				rc12.SetRenderTarget(m_downSamplingRT[rtIndex]);
				
				//シェーダーリソースビューと定数バッファをセットする。
				IShaderResourceDx12* srvTbl[] = {
					&prevRt->GetRenderTargetTexture()
				};
				CConstantBufferDx12* cbrTbl[] = {
					&m_blurParamCB
				};
				rc12.SetCBR_SRV_UAV(
					m_downSamplingDescriptorHeap[rtIndex].Get(),
					1,
					cbrTbl,
					1,
					srvTbl
				);
				rc12.DrawIndexed(4);
				rc12.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
		/*	prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
			//Yブラー。
			{
				//これダメかも？
				m_blurParam.offset.x = 0.0f;
				m_blurParam.offset.y = 16.0f / prevRt->GetWidth();
				m_blurParamCB.Update(&m_blurParam);

				rc12.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				D3D12_VIEWPORT viewport;
				viewport.TopLeftX = 0;
				viewport.TopLeftY = 0;
				viewport.Width = static_cast<float>(m_downSamplingRT[rtIndex].GetWidth());
				viewport.Height = static_cast<float>(m_downSamplingRT[rtIndex].GetHeight());
				viewport.MinDepth = D3D12_MIN_DEPTH;
				viewport.MaxDepth = D3D12_MAX_DEPTH;
				rc12.SetViewport(viewport);
				rc12.SetRenderTarget(m_downSamplingRT[rtIndex]);
				rc12.SetPipelineState(m_yblurLuminancePipelineState);
				//シェーダーリソースビューと定数バッファをセットする。
				IShaderResourceDx12* srvTbl[] = {
					&prevRt->GetRenderTargetTexture()
				};
				CConstantBufferDx12* cbrTbl[] = {
					&m_blurParamCB
				};
				rc12.SetCBR_SRV_UAV(
					m_downSamplingDescriptorHeap[rtIndex].Get(),
					1,
					cbrTbl,
					1,
					srvTbl
				);
				rc12.DrawIndexed(4);
				rc12.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;*/
		}
	}
	void CBloomDx12::Render(IRenderContext& rc)
	{
		auto& rc12 = rc.As<CRenderContextDx12>();
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();

		//重みを更新。
		UpdateWeight(25.0f);

		//頂点バッファを設定。
		rc12.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//インデックスバッファを設定。
		rc12.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//プリミティブトポロジーを設定する。
		rc12.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());

		//輝度を抽出する
		SamplingLuminance(ge12, rc12);
		//輝度テクスチャをぼかす。
		//BlurLuminanceTexture(ge12, rc12);

		rc12.WaitUntilToPossibleSetRenderTarget(ge12.GetMainRenderTarget());
		//レンダリングターゲットを輝度抽出用に切り替える。
		rc12.SetRenderTarget(ge12.GetMainRenderTarget());
		//ビューポートを戻す。
		rc12.SetViewport(ge12.GetMainRenderTargetViewport());
	}
}