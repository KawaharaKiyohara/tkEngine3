#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMaterialDx12.h"

namespace tkEngine {
	//ルートシグネチャとパイプラインステート周りはカリカリカリ
	enum {
		enDescriptorHeap_CB,
		enDescriptorHeap_SRV,
		enNumDescriptorHeap
	};
	
	void CMaterialDx12::InitTexture(const CTkmFile::SMaterial& tkmMat)
	{
		if (tkmMat.albedoMap != nullptr) {
			m_albedoMap.InitFromMemory(tkmMat.albedoMap.get(), tkmMat.albedoMapSize);
		}
		if (tkmMat.normalMap != nullptr) {
			m_normalMap.InitFromMemory(tkmMat.normalMap.get(), tkmMat.normalMapSize);
		}
		if (tkmMat.specularMap != nullptr) {
			m_specularMap.InitFromMemory(tkmMat.specularMap.get(), tkmMat.specularMapSize);
		}
	}
	void CMaterialDx12::InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat)
	{
		//todo シェーダーは仮。
#if 1
		m_vs.Load(L"shader/ModelPBR.fx", "VSMain", "vs_5_0");
		m_vsNonSkin.Load(L"shader/ModelPBR.fx", "VSMainNonSkin", "vs_5_0");
		m_ps.Load(L"shader/ModelPBR.fx", "PSMain", "ps_5_0");
#else
		m_vs.Load(L"shader/ModelSimple.fx", "VSMain", "vs_5_0");
		m_vsNonSkin.Load(L"shader/ModelSimple.fx", "VSMainNonSkin", "vs_5_0");
		m_ps.Load(L"shader/ModelSimple.fx", "PSMain", "ps_5_0");
#endif
		//テクスチャをロード。
		InitTexture(tkmMat);
		//ルートシグネチャを作成。
		InitRootSignature(tkmMat);
		//パイプラインステートを作成。
		InitPipelineState(tkmMat);
		//定数バッファを作成。
		
		SMaterialParam matParam;
		matParam.hasNormalMap = m_normalMap.IsValid() ? 1 : 0;
		matParam.hasSpecMap = m_specularMap.IsValid() ? 1 : 0;
		m_constantBuffer.Init(sizeof(SMaterialParam), &matParam);
	}
	void CMaterialDx12::InitRootSignature(const CTkmFile::SMaterial& tkmMat)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_DESCRIPTOR_RANGE1 ranges[enNumDescriptorHeap];
		CD3DX12_ROOT_PARAMETER1 rootParameters[enNumDescriptorHeap];

		ranges[enDescriptorHeap_CB].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[enDescriptorHeap_CB].InitAsDescriptorTable(1, &ranges[0]);
		ranges[enDescriptorHeap_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 32, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(1, &ranges[1]);

		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	}
	void CMaterialDx12::InitPipelineState(const CTkmFile::SMaterial& tkmMat)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
#if 1
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
#else
		// 頂点レイアウトを定義する。
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
#endif

		//パイプラインステートを作成。
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {0};
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob().Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = TRUE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
		//続いてスキンなしモデル用を作成。
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsNonSkin.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineStateNonSkin));
	}
	
	void CMaterialDx12::BeginRender(IRenderContext& rc, int hasSkin)
	{
		//ルートシグネチャとパイプラインステートを設定。
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		
		rcDx12.SetRootSignature(m_rootSignature);
		if (hasSkin) {
			rcDx12.SetPipelineState(m_pipelineState);
		}
		else {
			rcDx12.SetPipelineState(m_pipelineStateNonSkin);
		}
	}

}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12