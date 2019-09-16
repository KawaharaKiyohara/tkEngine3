#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// ���_�o�b�t�@�̃C���^�[�t�F�[�X�B
	/// </summary>
	class CVertexBufferDx12 : public IVertexBuffer {
		
	public:
		
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
		/// <param name="stride">���_�o�b�t�@�̃X�g���C�h�B��̒��_�̃T�C�Y</param>
		void Init(int size, int stride);
		/// <summary>
		/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
		void Copy(void* srcVertices) override final;
		/// <summary>
		/// ���_�o�b�t�@�r���[���擾�B
		/// </summary>
		/// <returns>�r���[</returns>
		D3D12_VERTEX_BUFFER_VIEW& GetView()
		{
			return m_vertexBufferView;
		}
	private:
		ComPtr<ID3D12Resource> m_vertexBuffer;			//���_�o�b�t�@�B
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;	//���_�o�b�t�@�r���[�B
	};
}