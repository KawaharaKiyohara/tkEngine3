#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"

namespace tkEngine {
	void CVertexBufferDx12::Init(int size, int stride)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer));
		//頂点バッファの作成に失敗しました。
		TK_ASSERT(SUCCEEDED(hr), "頂点バッファの作成に失敗しました。");
		//頂点バッファのビューを作成。
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.SizeInBytes = size;
		m_vertexBufferView.StrideInBytes = stride;
	}
	void CVertexBufferDx12::Copy(void* srcVertices)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		DirectX::ResourceUploadBatch re(d3dDevice.Get());
		re.Begin();
		D3D12_SUBRESOURCE_DATA subResourceData;
		subResourceData.pData = srcVertices;
		subResourceData.RowPitch = m_vertexBufferView.SizeInBytes;
		subResourceData.SlicePitch = 1;
		re.Upload(
			m_vertexBuffer.Get(),
			0,
			&subResourceData,
			1);

		re.End(ge12.GetCommandQueue().Get());

	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12