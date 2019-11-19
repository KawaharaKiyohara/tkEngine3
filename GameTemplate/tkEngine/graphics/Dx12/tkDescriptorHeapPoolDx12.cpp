#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDescriptorHeapPoolDx12.h"

namespace tkEngine {
	void CDescriptorHeapPoolDx12::Init()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();

		//�f�B�X�N���v�^�S�̃q�[�v���쐬�B
		m_cbvSrvUavBufferDescriptor4.resize(NUM_DESCRIPTOR_HEAP_4);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_4; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor4.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4);
		}
		//�f�B�X�N���v�^�W�̃q�[�v���쐬�B
		m_cbvSrvUavBufferDescriptor8.resize(NUM_DESCRIPTOR_HEAP_8);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_8; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor8.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 8);
		}
		//�f�B�X�N���v�^�P�U�̃q�[�v���쐬�B
		m_cbvSrvUavBufferDescriptor16.resize(NUM_DESCRIPTOR_HEAP_16);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_16; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor16.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 16);
		}
		//�f�B�X�N���v�^�R�Q�̃q�[�v���쐬�B
		m_cbvSrvUavBufferDescriptor32.resize(NUM_DESCRIPTOR_HEAP_32);
		for (int i = 0; i < NUM_DESCRIPTOR_HEAP_32; i++) {
			auto& heap = m_cbvSrvUavBufferDescriptor32.at(i);
			heap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 32);
		}
	}
}
