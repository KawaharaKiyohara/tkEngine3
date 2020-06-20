#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkPipelineStatesDx12.h"

namespace tkEngine {
	CShaderDx12 CPipelineStatesDx12::m_skinModelVS;    //スキンモデル用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_skinModelPS;    //スキンモデル用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_nonSkinModelVS; //ノンスキンモデル用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_skinModelShadowMapVS;		//スキンモデルのシャドウマップ書き込み用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_nonSkinModelShadowMapVS;	//ノンスキンモデルのシャドウマップ書き込み用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_modelShadowMapPS;			//モデルのシャドウマップ書き込み用のピクセルシェーダー。
	CShaderDx12 CPipelineStatesDx12::m_spriteVS;			//スプライト描画用の頂点シェーダー。
	CShaderDx12 CPipelineStatesDx12::m_spritePS;			//スプライト描画用のピクセルシェーダー。
	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_skinModelPipeline;    //スキンモデル描画用のパイプライン。
	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_nonSkinModelPipeline; //ノンスキンモデル用の場イプライン。
	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_spritePipeline;
	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_skinModelShadowMapPipeline;		//スキンモデルのシャドウマップ書き込み用のパイプライン。
	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_nonSkinModelShadowMapPipeline;	//ノンスキンモデルのシャドウマップ書き込み用のパイプライン。
	CRootSignatureDx12 CPipelineStatesDx12::m_modelDrawRootSignature;

	ComPtr<ID3D12PipelineState> CPipelineStatesDx12::m_copyMainTargetToFrameBufferPipeline;

	void CPipelineStatesDx12::Init()
	{
		InitShaders();
		InitRootSignature();
		InitPipelineState();
	}
	void CPipelineStatesDx12::InitShaders()
	{
#if 1
		m_skinModelVS.Load(L"shader/ModelPBR.fx", "VSMain", g_vsShaderModelName);
		m_nonSkinModelVS.Load(L"shader/ModelPBR.fx", "VSMainNonSkin", g_vsShaderModelName);
		m_skinModelPS.Load(L"shader/ModelPBR.fx", "PSMain", g_psShaderModelName);

		m_skinModelShadowMapVS.Load(L"shader/ModelPBR.fx", "VSMainSkinShadowMap", g_vsShaderModelName);
		m_nonSkinModelShadowMapVS.Load(L"shader/ModelPBR.fx", "VSMainNonSkinShadowMap", g_vsShaderModelName);
		m_modelShadowMapPS.Load(L"shader/ModelPBR.fx", "PSMainShadowMap", g_psShaderModelName);
#else
		m_skinModelVS.Load(L"shader/ModelSimple.fx", "VSMain", g_vsShaderModelName);
		m_nonSkinModelVS.Load(L"shader/ModelSimple.fx", "VSMainNonSkin", g_vsShaderModelName);
		m_skinModelPS.Load(L"shader/ModelSimple.fx", "PSMain", g_psShaderModelName);
#endif
		//シェーダーをロード。
		m_spriteVS.Load(L"shader/sprite.fx", "VSMain", g_vsShaderModelName);
		m_spritePS.Load(L"shader/sprite.fx", "PSMain", g_psShaderModelName);
	}
	
	void CPipelineStatesDx12::InitRootSignature()
	{
		//ルートシグネチャを作成。
		m_modelDrawRootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP
		);
	}
	void CPipelineStatesDx12::InitPipelineState()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		//モデル用のパイプラインステートを初期化。
		{
			// 頂点レイアウトを定義する。
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_modelDrawRootSignature.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_skinModelVS.GetCompiledBlob().Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_skinModelPS.GetCompiledBlob().Get());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = TRUE;
			psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
			psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			psoDesc.SampleDesc.Count = 1;
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_skinModelPipeline));
			//続いてスキンなしモデル用を作成。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_nonSkinModelVS.GetCompiledBlob().Get());
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_nonSkinModelPipeline));
			
			//シャドウマップ用のパイプラインステートを作成。
			//まずはスキンあり。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_skinModelShadowMapVS.GetCompiledBlob().Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_modelShadowMapPS.GetCompiledBlob().Get());
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R32_FLOAT;
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_skinModelShadowMapPipeline));
			//続いてスキンなし。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(CPipelineStatesDx12::m_nonSkinModelShadowMapVS.GetCompiledBlob().Get());
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_nonSkinModelShadowMapPipeline));
		}
		//スプライト描画用のパイプラインステートを初期化。
		{
			// 頂点レイアウトを定義する。
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			//パイプラインステートを作成。
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_modelDrawRootSignature.Get(); //モデル描画と共通でも問題ねーわ。
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_spriteVS.GetCompiledBlob().Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_spritePS.GetCompiledBlob().Get());
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
			psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			psoDesc.SampleDesc.Count = 1;
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_spritePipeline));

			//メインレンダリングターゲットからフレームバッファに書き込む用のパイプラインステートを作成。
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_copyMainTargetToFrameBufferPipeline));

		}
	}
}