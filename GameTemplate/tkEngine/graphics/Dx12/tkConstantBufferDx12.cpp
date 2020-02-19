#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkConstantBufferDx12.h"

namespace tkEngine {

	CConstantBufferDx12::~CConstantBufferDx12()
	{
		//�A���}�[�b�v
		CD3DX12_RANGE readRange(0, 0);
		for (auto& cb : m_constantBuffer) {
			cb->Unmap(0, &readRange);
		}
	}
	void CConstantBufferDx12::Init(int size, void* srcData)
	{
		m_size = size;
		//Dx12�ł̃O���t�B�b�N�X�G���W���Ɍ^�ϊ��B
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//D3D�f�o�C�X���擾�B
		auto device = geDx12.GetD3DDevice();


		//�萔�o�b�t�@��256�o�C�g�A���C�����g���v�������̂ŁA256�̔{���ɐ؂�グ��B
		m_allocSize = (size + 256) & 0xFFFFFF00;
		//�萔�o�b�t�@�̍쐬�B
		int bufferNo = 0;
		for( auto& cb : m_constantBuffer ){
			device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_allocSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&cb)
			);
			//�萔�o�b�t�@��CPU����A�N�Z�X�\�ȉ��z�A�h���X��ԂɃ}�b�s���O����B
			//�}�b�v�A�A���}�b�v�̃I�[�o�[�w�b�h���y�����邽�߂ɂ͂��̃C���X�^���X�������Ă���Ԃ͍s��Ȃ��B
			{
				CD3DX12_RANGE readRange(0, 0);        //     intend to read from this resource on the CPU.
				cb->Map(0, &readRange, reinterpret_cast<void**>(&m_constBufferCPU[bufferNo]));
			}
			if (srcData != nullptr) {
				memcpy(m_constBufferCPU[bufferNo], srcData, m_size);
			}
			bufferNo++;
		}
	}
	void CConstantBufferDx12::RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
	{
		//Dx12�ł̃O���t�B�b�N�X�G���W���Ɍ^�ϊ��B
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();

		RegistConstantBufferView(descriptorHandle, backBufferIndex);
	}
	void CConstantBufferDx12::RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
	{
		//D3D�f�o�C�X���擾�B
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = geDx12.GetD3DDevice();
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_constantBuffer[bufferNo]->GetGPUVirtualAddress();
		desc.SizeInBytes = m_allocSize;
		device->CreateConstantBufferView(&desc, descriptorHandle);
	}
	void CConstantBufferDx12::Update(void* data)
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		memcpy(m_constBufferCPU[backBufferIndex], data, m_size);
	}
}
