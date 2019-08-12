#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine{
	/// <summary>
	/// �v���~�e�B�u�N���X�B
	/// </summary>
	class CPrimitive : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="srcVertexData">���_�o�b�t�@�̃\�[�X�f�[�^�B</param>
		/// <param name="vertexBufferSize">���_�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="vertexBufferStride">���_�o�b�t�@�̃X�g���C�h�B</param>
		/// <param name="srcIndexData">�C���f�b�N�X�o�b�t�@�̃\�[�X�f�[�^�B</param>
		/// <param name="indexBufferSize">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="indexBufferStride">�C���f�b�N�X�o�b�t�@�̃X�g���C�h�B</param>
		/// <param name="enPrimTopology">�v���~�e�B�u�̃g�|���W�[�B</param>
		/// <returns></returns>
		void Init(
			void* srcVertexData,
			int vertexBufferSize,
			int vertexBufferStride,
			void* srcIndexData,
			int indexBufferSize,
			int indexBufferStride,
			EnPrimitiveTopology enPrimTopology);
		/// <summary>
		/// ���_�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		UPIVertexBuffer& GetVertexBuffer() 
		{
			return m_vertexBuffer;
		}
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		UPIIndexBuffer& GetIndexBuffer()
		{
			return m_indexBuffer;
		}
		/// <summary>
		/// �v���~�e�B�u�̃g�|���W�[���擾�B
		/// </summary>
		/// <returns></returns>
		EnPrimitiveTopology GetPrimitiveTopology() const
		{
			return m_topology;
		}
	protected:
		UPIVertexBuffer	m_vertexBuffer;	//���_�o�b�t�@�B
		UPIIndexBuffer m_indexBuffer;	//�C���f�b�N�X�o�b�t�@�B
		EnPrimitiveTopology m_topology = enPrimitiveTopology_Undex;	//�g�|���W�[�B
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};
}