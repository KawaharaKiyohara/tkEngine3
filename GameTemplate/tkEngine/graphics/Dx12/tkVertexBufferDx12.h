#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// ���_�o�b�t�@�̃C���^�[�t�F�[�X�B
	/// </summary>
	class CVertexBuffer : public IVertexBuffer {
		
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">���_�o�b�t�@�̃X�g���C�h�B��̒��_�̃T�C�Y�B</param>
		CVertexBuffer(int size, int stride);
		/// <summary>
		/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
		void Copy(void* srcVertices) override final;
	private:
		ComPtr<ID3D12Resource> m_vertexBuffer;			//���_�o�b�t�@�B
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;	//���_�o�b�t�@�r���[�B
	};
}