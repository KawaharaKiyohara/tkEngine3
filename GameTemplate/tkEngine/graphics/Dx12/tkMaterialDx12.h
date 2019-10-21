#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkTkmFile.h"
namespace tkEngine {
	/// <summary>
	/// �}�e���A���B
	/// </summary>
	class CMaterialDx12 : Noncopyable {
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
		/// <param name="hasSkin">�X�L�������邩�ǂ����̃t���O</param>
		void BeginRender(IRenderContext& rc, int hasSkin);

		/// <summary>
		/// �A���x�h�}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetAlbedoMap()
		{
			return m_albedoMap;
		}
		/// <summary>
		/// �@���}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetNormalMap()
		{
			return m_normalMap;
		}
		/// <summary>
		/// �X�y�L�����}�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetSpecularMap()
		{
			return m_specularMap;
		}
		/// <summary>
		/// �萔�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		CConstantBufferDx12& GetConstantBuffer()
		{
			return m_constantBuffer;
		}
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
		/// <summary>
		/// �e�N�X�`�����������B
		/// </summary>
		/// <param name="tkmMat"></param>
		void InitTexture(const CTkmFile::SMaterial& tkmMat);
	private:
		/// <summary>
		/// �}�e���A���p�����[�^�B
		/// </summary>
		struct SMaterialParam {
			int hasNormalMap;	//�@���}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
			int hasSpecMap;		//�X�y�L�����}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
		};
		CShaderDx12		m_vsNonSkin;	//�X�L���Ȃ����f���̒��_�V�F�[�_�[�B
		CShaderDx12		m_vs;			//���_�V�F�[�_�[�B
		CShaderDx12		m_ps;			//�s�N�Z���V�F�[�_�[�B
		CTextureDx12	m_albedoMap;	//�A���x�h�}�b�v�B
		CTextureDx12	m_normalMap;	//�@���}�b�v�B
		CTextureDx12	m_specularMap;	//�X�y�L�����}�b�v�B
		ComPtr<ID3D12RootSignature> m_rootSignature;		//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;			//�p�C�v���C���X�e�[�g�B
		ComPtr<ID3D12PipelineState> m_pipelineStateNonSkin;		//�X�L���Ȃ����f����`�悷�鎞�̃p�C�v���C���X�e�[�g�B
		CConstantBufferDx12 m_constantBuffer;	//�萔�o�b�t�@�B
	};

	using UPCMaterialDx12 = unique_ptr< CMaterialDx12>;
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

