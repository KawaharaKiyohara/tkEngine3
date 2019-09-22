#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkTkmFile.h"
namespace tkEngine {
	/// <summary>
	/// �}�e���A���B
	/// </summary>
	class CMaterialDx12 : Noncopyable{
	public:
		/// <summary>
		/// tkm�t�@�C���̃}�e���A����񂩂珉��������B
		/// </summary>
		/// <param name="tkmMat">tkm�}�e���A��</param>
		void InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat);
		/// <summary>
		/// �����_�����O���J�n����Ƃ��ɌĂяo���֐��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void BeginRender(IRenderContext& rc);
		/// <summary>
		/// �����_�����O���J�n����Ƃ��ɌĂяo���֐��B
		/// </summary>
		/// <param name="rc"></param>
		void EndRender(IRenderContext& rc);
	private:
		/// <summary>
		/// �p�C�v���C���X�e�[�g���������B
		/// </summary>
		/// <param name="tkmMat">tkm�}�e���A��</param>
		void InitPipelineState(const CTkmFile::SMaterial& tkmMat);
		/// <summary>
		/// ���[�g�V�O�l�`�����������B
		/// </summary>
		/// <param name="tkmMat">tkmMat</param>
		void InitRootSignature(const CTkmFile::SMaterial& tkmMat);
	private:
		CShaderDx12		m_vs;			//���_�V�F�[�_�[�B
		CShaderDx12		m_ps;			//�s�N�Z���V�F�[�_�[�B
		CTextureDx12	m_albedoMap;	//�A���x�h�}�b�v�B
		CTextureDx12	m_normalMap;	//�@���}�b�v�B
		CTextureDx12	m_specularMap;	//�X�y�L�����}�b�v�B
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};

	using UPCMaterialDx12 = std::unique_ptr< CMaterialDx12>;
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

