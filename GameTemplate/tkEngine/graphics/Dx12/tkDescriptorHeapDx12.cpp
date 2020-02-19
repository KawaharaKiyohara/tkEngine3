#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDescriptorHeapDx12.h"


namespace tkEngine {

	void CDescriptorHeapDx12::Init(D3D12_DESCRIPTOR_HEAP_TYPE type)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = MAX_SHADER_RESOURCE + MAX_CONSTANT_BUFFER;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap[0]));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");
		hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap[1]));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");
	}
	ID3D12DescriptorHeap* CDescriptorHeapDx12::Get()
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_descriptorHeap[backBufferIndex].Get();
	}
	D3D12_GPU_DESCRIPTOR_HANDLE CDescriptorHeapDx12::GetConstantBufferGpuDescritorStartHandle() const
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_cbGpuDescriptorStart[backBufferIndex];
	}
	D3D12_GPU_DESCRIPTOR_HANDLE CDescriptorHeapDx12::GetShaderResourceGpuDescritorStartHandle() const
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_srGpuDescriptorStart[backBufferIndex];
	}
	void CDescriptorHeapDx12::Commit()
	{
		int bufferNo = 0;
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		for (auto& descriptorHeap : m_descriptorHeap) {
			auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

			//�萔�o�b�t�@��o�^���Ă����B
			for (auto& cb : m_constantBuffers) {
				if (cb != nullptr) {
					cb->RegistConstantBufferView(cpuHandle, bufferNo);
				}
				//���ɐi�߂�B
				cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
			}

			//�����ăV�F�[�_�[���\�[�X�B
			for (auto& sr : m_shaderResources) {
				if (sr != nullptr) {
					sr->RegistShaderResourceView(cpuHandle, bufferNo);
				}
				//���ɐi�߂�B
				cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
			}

			//�萔�o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h�����v�Z�B
			m_cbGpuDescriptorStart[bufferNo] = gpuHandle;
			//�V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h�����v�Z�B
			m_srGpuDescriptorStart[bufferNo] = gpuHandle;
			m_srGpuDescriptorStart[bufferNo].ptr += (UINT64)ge12.GetCbrSrvDescriptorSize() * MAX_CONSTANT_BUFFER;
			bufferNo++;
		}
	}
}