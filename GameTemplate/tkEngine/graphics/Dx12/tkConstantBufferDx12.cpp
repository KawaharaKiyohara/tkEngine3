#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkConstantBufferDx12.h"

namespace tkEngine {

	CConstantBufferDx12::~CConstantBufferDx12()
	{
		//�A���}�[�b�v
		CD3DX12_RANGE readRange(0, 0);
		m_constantBuffer->Unmap(0, &readRange);
	}
	void CConstantBufferDx12::Init(int size, void* srcData)
	{
		m_size = size;
		//Dx12�ł̃O���t�B�b�N�X�G���W���Ɍ^�ϊ��B
		auto geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//D3D�f�o�C�X���擾�B
		auto device = geDx12->GetD3DDevice();


		//�萔�o�b�t�@��256�o�C�g�A���C�����g���v�������̂ŁA256�̔{���ɐ؂�グ��B
		m_allocSize = (size + 256) & 0xFFFFFF00;
		//�萔�o�b�t�@�̍쐬�B
		{
			device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(m_allocSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_constantBuffer)
			);
		}
		
		//�萔�o�b�t�@��CPU����A�N�Z�X�\�ȉ��z�A�h���X��ԂɃ}�b�s���O����B
		//�}�b�v�A�A���}�b�v�̃I�[�o�[�w�b�h���y�����邽�߂ɂ͂��̃C���X�^���X�������Ă���Ԃ͍s��Ȃ��B
		{
			CD3DX12_RANGE readRange(0, 0);        //     intend to read from this resource on the CPU.
			m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_constBufferCPU));
		}
		if (srcData != nullptr) {
			Update(srcData);
		}
	}
	void CConstantBufferDx12::RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
	{
		//Dx12�ł̃O���t�B�b�N�X�G���W���Ɍ^�ϊ��B
		auto geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//D3D�f�o�C�X���擾�B
		auto device = geDx12->GetD3DDevice();
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
		desc.SizeInBytes = m_allocSize;
		device->CreateConstantBufferView(&desc, descriptorHandle);
	}
	void CConstantBufferDx12::Update(void* data)
	{
		memcpy(m_constBufferCPU, data, m_size);
	}
}
