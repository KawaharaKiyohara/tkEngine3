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
		/// �f�B�X�N���v�^�q�[�v�̎擾�B
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12DescriptorHeap> GetDiscriptorHeap() const
		{
			return m_cbvHeap;
		}
    private:
		ComPtr<ID3D12Resource>	m_constantBuffer;	//�萔�o�b�t�@�B
		ComPtr<ID3D12DescriptorHeap> m_cbvHeap;		//�f�B�X�N���v�^�q�[�v�B
		void* m_constBufferCPU = nullptr;			//CPU������A�N�Z�X�ł��邷��萔�o�b�t�@�̃A�h���X�B
		int m_size = 0;								//�萔�o�b�t�@�̃T�C�Y�B
    };
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12