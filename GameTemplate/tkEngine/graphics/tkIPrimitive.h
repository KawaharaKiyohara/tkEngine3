#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine{
	/// <summary>
	/// �v���~�e�B�u�̃C���^�[�t�F�[�X�B
	/// </summary>
	class IPrimitive : Noncopyable {
	public:
		/// <summary>
		/// �v���~�e�B�u���쐬�B
		/// </summary>
		/// <param name="vertexBufferSize">���_�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="vertexBufferStride">���_�o�b�t�@�̃X�g���C�h�B</param>
		/// <param name="indexBufferSize">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="indexBufferStride">�C���f�b�N�X�o�b�t�@�̃X�g���C�h�B</param>
		/// <param name="enPrimTopology">�v���~�e�B�u�̃g�|���W�[�B</param>
		/// <returns></returns>
		static std::unique_ptr<IPrimitive> Create(
			int vertexBufferSize,
			int vertexBufferStride,
			int indexBufferSize,
			int indexBufferStride,
			EnPrimitiveTopology enPrimTopology );
		/// <summary>
		/// �`��B
		/// </summary>
		virtual void Draw(CRenderContext& rc) = 0;
	protected:
		UPIVertexBuffer	m_vertexBuffer;	//���_�o�b�t�@�B
		UPIIndexBuffer m_indexBuffer;	//�C���f�b�N�X�o�b�t�@�B
		EnPrimitiveTopology m_topology = enPrimitiveTopology_Undex;	//�g�|���W�[�B
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};
}