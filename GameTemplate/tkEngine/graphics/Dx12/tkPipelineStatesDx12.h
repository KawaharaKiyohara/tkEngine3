#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkRootSignatureDx12.h"

namespace tkEngine{
    class CPipelineStatesDx12 {
    public:
        
        static void Init();
		static CRootSignatureDx12 m_modelDrawRootSignature;	//���f���`��p�̃��[�g�V�O�l�`���B
        static CShaderDx12 m_skinModelVS;		//�X�L�����f���p�̒��_�V�F�[�_�[�B
        static CShaderDx12 m_skinModelPS;		//�X�L�����f���p�̒��_�V�F�[�_�[�B
        static CShaderDx12 m_nonSkinModelVS;	//�m���X�L�����f���p�̒��_�V�F�[�_�[�B
		static CShaderDx12 m_skinModelShadowMapVS;		//�X�L�����f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
		static CShaderDx12 m_nonSkinModelShadowMapVS;	//�m���X�L�����f���̃V���h�E�}�b�v�������ݗp�̒��_�V�F�[�_�[�B
		static CShaderDx12 m_modelShadowMapPS;			//���f���̃V���h�E�}�b�v�������ݗp�̃s�N�Z���V�F�[�_�[�B
		static CShaderDx12 m_spriteVS;			//�X�v���C�g�`��p�̒��_�V�F�[�_�[�B
		static CShaderDx12 m_spritePS;			//�X�v���C�g�`��p�̃s�N�Z���V�F�[�_�[�B
        static ComPtr<ID3D12PipelineState> m_skinModelPipeline;			//�X�L�����f���`��p�̃p�C�v���C���B
        static ComPtr<ID3D12PipelineState> m_nonSkinModelPipeline;		//�m���X�L�����f���p�̃p�C�v���C���B
		static ComPtr<ID3D12PipelineState> m_skinModelShadowMapPipeline;		//�X�L�����f���̃V���h�E�}�b�v�������ݗp�̃p�C�v���C���B
		static ComPtr<ID3D12PipelineState> m_nonSkinModelShadowMapPipeline;	//�m���X�L�����f���̃V���h�E�}�b�v�������ݗp�̃p�C�v���C���B
		static ComPtr<ID3D12PipelineState> m_spritePipeline;		//�X�v���C�g�p�̃p�C�v���C���B
		static ComPtr<ID3D12PipelineState> m_copyMainTargetToFrameBufferPipeline;	//���C�������_�����O�^�[�Q�b�g����t���[���o�b�t�@�ɃR�s�[����p�̃p�C�v���C���B
	private:
		/// <summary>
		/// �V�F�[�_�[���������B
		/// </summary>
		static void InitShaders();
		/// <summary>
		/// ���[�g�V�O�l�`�����������B
		/// </summary>
		static void InitRootSignature();
		/// <summary>
		/// �p�C�v���C���X�e�[�g�̏������B
		/// </summary>
		static void InitPipelineState();
    };
}
#endif