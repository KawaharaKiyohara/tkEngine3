#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkPrimitiveDx12.h"
#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#include "tkEngine/graphics/Dx12/d3dx12.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"

namespace tkEngine {
	CPrimitiveDx12::CPrimitiveDx12()
	{
		auto gfxEngineDx12 = g_graphicsEngine->GetImplement<CGraphicsEngineDx12>();
		auto d3dDevice = gfxEngineDx12->GetD3DDevice();

		//空のルートシグネチャを作成。
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	}
	void CPrimitiveDx12::Draw(CRenderContext& rc)
	{
		auto commandList = rc.GetCommandList();
		//プリミティブトポロジーを設定。
		commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(m_topology));
		//Dx12版の頂点バッファに型変換。
		auto vbDx12 = m_vertexBuffer->As<CVertexBufferDx12>();
		auto ibDx12 = m_indexBuffer->As<CIndexBufferDx12>();
		//頂点バッファを設定。
		commandList->IASetVertexBuffers(0, 1, &vbDx12->GetView());
		//インデックスバッファを設定。
		commandList->IASetIndexBuffer(&ibDx12->GetView());
		//どろー
		commandList->DrawIndexedInstanced(m_indexBuffer->GetCount(), 1, 0, 0, 0);
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12