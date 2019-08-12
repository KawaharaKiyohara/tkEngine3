#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�B
	/// </summary>
	class CIndexBufferDx12 : public IIndexBuffer {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�X�g���C�h�B</param>
		CIndexBufferDx12(int size, int stride);
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		virtual ~CIndexBufferDx12() {}
		/// <summary>
		/// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcIndecies">�R�s�[���̃C���f�b�N�X�f�[�^�B</param>
		void Copy(void* srcIndecies) override final;
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�r���[���擾�B
		/// </summary>
		/// <returns></returns>
		D3D12_INDEX_BUFFER_VIEW& GetView()
		{
			return m_indexBufferView;
		}
	private:
		ComPtr<ID3D12Resource> m_indexBuffer;			//�C���f�b�N�X�o�b�t�@�B
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;	//�C���f�b�N�X�o�b�t�@�r���[�B
	};
}

#endif // #if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12