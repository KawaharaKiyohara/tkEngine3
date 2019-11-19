#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDescriptorHeapPoolDx12.h"

namespace tkEngine {
	void CDescriptorHeapPoolDx12::Init()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();

		//ディスクリプタ４個のヒープを作成。
		m_cbvSrvUavBufferDescriptor4.resize(NUM_DESCRIPTOR_HEAP_4);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_4; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor4.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4);
		}
		//ディスクリプタ８個のヒープを作成。
		m_cbvSrvUavBufferDescriptor8.resize(NUM_DESCRIPTOR_HEAP_8);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_8; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor8.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 8);
		}
		//ディスクリプタ１６個のヒープを作成。
		m_cbvSrvUavBufferDescriptor16.resize(NUM_DESCRIPTOR_HEAP_16);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_16; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor16.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 16);
		}
		//ディスクリプタ３２個のヒープを作成。
		m_cbvSrvUavBufferDescriptor32.resize(NUM_DESCRIPTOR_HEAP_32);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_32; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor32.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 32);
		}
	}
}
