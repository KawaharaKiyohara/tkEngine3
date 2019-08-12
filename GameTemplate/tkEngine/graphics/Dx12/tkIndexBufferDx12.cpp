#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/Dx12/d3dx12.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"

namespace tkEngine {
	CIndexBufferDx12::CIndexBufferDx12(int size, int stride)
	{
		auto gfxEngine12 = g_graphicsEngine->GetImplement<CGraphicsEngineDx12>();
		auto d3dDevice = gfxEngine12->GetD3DDevice();
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_indexBuffer));
		//頂点バッファの作成に失敗しました。
		TK_ASSERT(SUCCEEDED(hr), "インデックスバッファの作成に失敗しました。");
		//インデックスバッファのビューを作成。
		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = size;
		if (stride == 2) {
			//ストライドが２バイト。
			m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		}
		else if (stride == 4) {
			//ストライドが４バイト。
			m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		}
	}
	void CIndexBufferDx12::Copy(void* srcIndecies)
	{
		UINT8* pIndexDataBegin;
		CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
		m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pIndexDataBegin));
		memcpy(pIndexDataBegin, srcIndecies, m_indexBufferView.SizeInBytes);
		m_indexBuffer->Unmap(0, nullptr);
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12