#pragma once

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkRenderContextDx12.h"

namespace tkEngine {
	/// <summary>
	/// �u���[���B
	/// </summary>
	class CBloomDx12 : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="config">�O���t�B�b�N�ݒ�</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render(IRenderContext& rc);
	private:
		/// <summary>
		/// �d�݂��X�V�B
		/// </summary>
		/// <param name="dispersion"></param>
		void UpdateWeight(float dispersion);
		/// <summary>
		/// �����_�����O�^�[�Q�b�g���������B
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// �l�p�`�v���~�e�B�u���������B
		/// </summary>
		void InitQuadPrimitive();
		/// <summary>
		/// �V�F�[�_�[���������B
		/// </summary>
		void InitShaders();
		/// <summary>
		/// �p�C�v���C���X�e�[�g���������B
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// �P�x�𒊏o
		/// </summary>
		/// <param name="rc"></param>
		void CBloomDx12::SamplingLuminance(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12);
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̏������B
		/// </summary>
		void InitDescriptorHeap();
	private:
		static const int NUM_WEIGHTS = 8;
		static const int NUM_DOWN_SAMPLING_RT = 10;
		/// <summary>
		/// �u���[�p�̃p�����[�^�B
		/// </summary>
		struct SBlurParam {
			CVector4 offset;
			float weights[NUM_WEIGHTS];
		};
		ComPtr<ID3D12PipelineState> m_samplingLuminancePipelineState;	//�P�x���o�X�e�b�v�̃p�C�v���C���X�e�[�g�B
		CPrimitive m_quadPrimitive;			//�l�p�`�v���~�e�B�u�B
		CRenderTargetDx12 m_luminanceRT;	//�P�x�𒊏o���邽�߂̃����_�����O�^�[�Q�b�g�B
		CRenderTargetDx12 m_combineRT;		//�ڂ��������p�̃����_�����O�^�[�Q�b�g�B
		CRenderTargetDx12 m_downSamplingRT[NUM_DOWN_SAMPLING_RT];	//�_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g�B
		SBlurParam m_blurParam;			//�u���[�p�̃p�����[�^�B
		CConstantBufferDx12 m_cbBlur;
		CShaderDx12 m_vs;				//���_�V�F�[�_�[�B
		CShaderDx12 m_psLuminance;		//�P�x���o�p�X�̃s�N�Z���V�F�[�_�[�B
		CShaderDx12 m_vsXBlur;			//X�����u���[�p�X�̒��_�V�F�[�_�[�B
		CShaderDx12 m_vsYBlur;			//Y�����u���[�p�X�̒��_�V�F�[�_�[�B
		CShaderDx12 m_psBlur;			//X�����u���[/Y�����u���[�p�X�̃s�N�Z���V�F�[�_�[�B
		CShaderDx12 m_psCombine;		//�����p�X�̃s�N�Z���V�F�[�_�[�B
		CShaderDx12 m_copyVS;			//�R�s�[�p�̒��_�V�F�[�_�[�B	
		CShaderDx12 m_copyPS;			//�R�s�[�p�̃s�N�Z���V�F�[�_�[�B
		ComPtr< ID3D12DescriptorHeap> m_luminanceDescriptorHeap;	//�P�x���o���Ɏg���f�B�X�N���v�^�q�[�v�B

	};
}