#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"

namespace tkEngine {
	CVertexBufferDx12::CVertexBufferDx12(int size, int stride)
	{
		auto gfxEngine12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = gfxEngine12->GetD3DDevice();
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
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
		UINT8* pVertexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin));
		memcpy(pVertexDataBegin, srcVertices, m_vertexBufferView.SizeInBytes);
		m_vertexBuffer->Unmap(0, nullptr);

	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12