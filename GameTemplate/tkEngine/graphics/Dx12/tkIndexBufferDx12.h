#pragma once

#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�B
	/// </summary>
	class CIndexBuffer : public IIndexBuffer {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�X�g���C�h�B</param>
		CIndexBuffer(int size, int stride);
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		virtual ~CIndexBuffer() {}
		/// <summary>
		/// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcIndecies">�R�s�[���̃C���f�b�N�X�f�[�^�B</param>
		void Copy(void* srcIndecies) override final;
	private:
		ComPtr<ID3D12Resource> m_indexBuffer;			//�C���f�b�N�X�o�b�t�@�B
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;	//�C���f�b�N�X�o�b�t�@�r���[�B
	};
}