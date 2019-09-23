#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

namespace tkEngine{
    class CConstantBufferDx12 : Noncopyable{
    public:
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~CConstantBufferDx12();
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="size">�萔�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="srcData">�\�[�X�f�[�^�Bnull���w�肷�邱�Ƃ��\�B</param>
		void Init(int size, void* srcData);

		/// <summary>
		/// �萔�o�b�t�@�̒��g���X�V�B
		/// </summary>
		/// <param name="data"></param>
		void Update(void* data);
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v��ConstantBufferView��o�^�B
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
    private:
		ComPtr<ID3D12Resource>	m_constantBuffer;	//�萔�o�b�t�@�B
		void* m_constBufferCPU = nullptr;			//CPU������A�N�Z�X�ł��邷��萔�o�b�t�@�̃A�h���X�B
		int m_size = 0;								//�萔�o�b�t�@�̃T�C�Y�B
		int m_allocSize = 0;
    };
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12