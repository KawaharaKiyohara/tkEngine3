#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"

namespace tkEngine {
	CStructuredBufferDx12::~CStructuredBufferDx12()
	{
		//アンマーップ
		CD3DX12_RANGE readRange(0, 0);
		for (auto& buffer : m_buffersOnGPU) {
			buffer->Unmap(0, &readRange);
		}
	}
	void CStructuredBufferDx12::Init(int sizeOfElement, int numElement, void* initData)
	{
		m_sizeOfElement = sizeOfElement;
		m_numElement = numElement;
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = geDx12.GetD3DDevice();

		int bufferNo = 0;
		for (auto& buffer : m_buffersOnGPU) {
			device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_sizeOfElement* m_numElement),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&buffer)
			);

			//構造化バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする。
			//マップ、アンマップのオーバーヘッドを軽減するためにはこのインスタンスが生きている間は行わない。
			{
				CD3DX12_RANGE readRange(0, 0);        //     intend to read from this resource on the CPU.
				buffer->Map(0, &readRange, reinterpret_cast<void**>(&m_buffersOnCPU[bufferNo]));
			}
			if (initData != nullptr) {
				memcpy(m_buffersOnCPU[bufferNo], initData, m_sizeOfElement* m_numElement);
			}
			bufferNo++;
		}
		m_isInited = true;
	}
	void CStructuredBufferDx12::Update(void* data)
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		memcpy(m_buffersOnCPU[backBufferIndex], data, m_numElement * m_sizeOfElement);
	}
	void CStructuredBufferDx12::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		RegistShaderResourceView(descriptorHandle, ge12.GetBackBufferIndex());
	}

	void CStructuredBufferDx12::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
	{
		if (!m_isInited) {
			return;
		}
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();

		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Buffer.NumElements = static_cast<UINT>(m_numElement);
		srvDesc.Buffer.StructureByteStride = m_sizeOfElement;
		srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		device->CreateShaderResourceView(
			m_buffersOnGPU[bufferNo].Get(),
			&srvDesc,
			descriptorHandle
		);
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12