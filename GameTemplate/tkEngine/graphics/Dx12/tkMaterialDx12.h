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
		/// <param name="tkmMat">�}�e���A��</param>
		void InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat);
	private:
		CShaderDx12		m_vs;			//���_�V�F�[�_�[�B
		CShaderDx12		m_ps;			//�s�N�Z���V�F�[�_�[�B
		CTextureDx12	m_albedoMap;	//�A���x�h�}�b�v�B
		CTextureDx12	m_normalMap;	//�@���}�b�v�B
		CTextureDx12	m_specularMap;	//�X�y�L�����}�b�v�B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};

	using UPCMaterialDx12 = std::make_unique< CMaterialDx12>;
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

