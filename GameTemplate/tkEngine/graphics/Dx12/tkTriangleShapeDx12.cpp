#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/dx12/tkTriangleShapeDx12.h"
#include "tkEngine/graphics/Dx12/d3dx12.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"
#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"

namespace tkEngine {
	struct Vertex
	{
		CVector3 position;
		CVector4 color;
	};

	void CTriangleShapeDx12::Init()
	{
		auto gfxEngineDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = gfxEngineDx12->GetD3DDevice();
		//空のルートシグネチャを作成。
		{
			
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
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
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
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
			{ { 0.0f, 0.25f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { 0.25f, -0.25f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.25f, -0.25f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
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
	}
	void CTriangleShapeDx12::Draw(CRenderContext& rc)
	{

		auto commandList = rc.GetCommandList();
		//ルートシグネチャを設定。
		commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		commandList->SetPipelineState(m_pipelineState.Get());
		//プリミティブトポロジーを設定。
		commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_primitive.GetPrimitiveTopology()));
		//Dx12版の頂点バッファに型変換。
		auto vbDx12 = m_primitive.GetVertexBuffer()->As<CVertexBufferDx12>();
		auto ibDx12 = m_primitive.GetIndexBuffer()->As<CIndexBufferDx12>();
		//頂点バッファを設定。
		commandList->IASetVertexBuffers(0, 1, &vbDx12->GetView());
		//インデックスバッファを設定。
		commandList->IASetIndexBuffer(&ibDx12->GetView());
		//どろー
		commandList->DrawIndexedInstanced(ibDx12->GetCount(), 1, 0, 0, 0);
	}
}

#endif 