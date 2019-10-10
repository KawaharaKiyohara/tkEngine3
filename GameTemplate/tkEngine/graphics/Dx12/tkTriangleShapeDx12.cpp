#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkTriangleShapeDx12.h"

namespace tkEngine {
	struct Vertex
	{
		CVector3 position;
		CVector4 color;
		CVector2 uv;
	};

	CTriangleShapeDx12::CTriangleShapeDx12(const wchar_t* textureFilePath)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		//ルートシグネチャを作成。
		{
			D3D12_STATIC_SAMPLER_DESC sampler = {};
			sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
			sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
			sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
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

			ranges[enDescriptorHeap_CB].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			rootParameters[enDescriptorHeap_CB].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);
			ranges[enDescriptorHeap_SRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
			rootParameters[enDescriptorHeap_SRV].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			// Allow input layout and deny uneccessary access to certain pipeline stages.
			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS ;

			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 1, &sampler, rootSignatureFlags);
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
			d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
		}
		//パイプラインステートの作成とシェーダーのコンパイル。
		{
			m_vs.Load(L"shader/Primitive.fx", "VSMainNoTexture", "vs_5_0");
			m_ps.Load(L"shader/Primitive.fx", "PSMainNoTexture", "ps_5_0");

			// Define the vertex input layout.
			D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
			psoDesc.pRootSignature = m_rootSignature.Get();
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob().Get());
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.DepthStencilState.DepthEnable = FALSE;
			psoDesc.DepthStencilState.StencilEnable = FALSE;
			psoDesc.SampleMask = UINT_MAX;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.NumRenderTargets = 1;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;
			d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
		}
		//ここら辺は超適当。とりあえず絵を出す。
		//プリミティブを作成。
		Vertex triangleVertices[] =
		{
			{ { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, {0.5f, 0.0f } },
			{ { 1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, {0.0f, 1.0f } },
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, {1.0f, 1.0f} }
		};
		unsigned short indices[] = {
			0, 1, 2
		};
		m_primitive.Init(
			triangleVertices,
			sizeof(triangleVertices), 
			sizeof(triangleVertices[0]), 
			indices,
			sizeof(indices), 
			sizeof(indices[0]),
			enPrimitiveTopology_TriangleList
		);
		//定数バッファを作成。
		m_constantBuffer.Init(sizeof(SConstantBuffer), nullptr);

		//テクスチャが指定されている。
		if (textureFilePath != nullptr) {
			m_texture.InitFromDDSFile(textureFilePath);
		}
	}
	void CTriangleShapeDx12::Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj)
	{
		//定数バッファを更新。
		SConstantBuffer cb;
		cb.mWorld = m_worldMatrix;
		cb.mView = mView;
		cb.mProj = mProj;
		
		m_constantBuffer.Update(&cb);
		auto& rcDx12 = rc.As<CRenderContextDx12>();

		//ルートシグネチャを設定。
		rcDx12.SetRootSignature(m_rootSignature);
		rcDx12.SetPipelineState(m_pipelineState);
	
#if 0 //todo これ考え方を間違えていたぽい。
		//ディスクリプタテーブルを設定する。
		int numDescriporHeap = 1;
		ID3D12DescriptorHeap* ppHeaps[1] = { m_constantBuffer.GetDiscriptorHeap().Get() };
		commandList->SetDescriptorHeaps(1, ppHeaps);
		//ディスクリプタヒープをルートシグネチャに登録していく。
		commandList->SetGraphicsRootDescriptorTable(
			enDescriptorHeap_CB,
			ppHeaps[0]->GetGPUDescriptorHandleForHeapStart()
		);
		if (m_texture.IsValid()) {
			ppHeaps[0] = m_texture.GetDiscriptorHeap().Get();
			commandList->SetDescriptorHeaps(1, ppHeaps);
			commandList->SetGraphicsRootDescriptorTable(
				enDescriptorHeap_SRV,
				ppHeaps[0]->GetGPUDescriptorHandleForHeapStart()
			);
		}
#endif	

		//プリミティブトポロジーを設定。
		rcDx12.SetPrimitiveTopology(m_primitive.GetPrimitiveTopology());
		//Dx12版の頂点バッファに型変換。
		auto& ib = m_primitive.GetIndexBuffer();
		//頂点バッファを設定。
		rcDx12.SetVertexBuffer(m_primitive.GetVertexBuffer());
		//インデックスバッファを設定。
		rcDx12.SetIndexBuffer(ib);
		//どろー
		rcDx12.DrawIndexed(ib->GetCount());
	}
}

#endif 