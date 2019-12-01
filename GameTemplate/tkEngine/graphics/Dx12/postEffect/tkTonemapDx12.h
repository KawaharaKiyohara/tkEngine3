#pragma once

namespace tkEngine {
	/// <summary>
	/// �g�[���}�b�v�B
	/// </summary>
	class CTonemapDx12 : Noncopyable {
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
		/// ���ϋP�x�̌v�Z�B
		/// </summary>
		void CalcLuminanceAvarage(CRenderContextDx12& rc);
	public:
		static const int MAX_SAMPLES = 16;
	private:
		struct STonemapParam {
			float deltaTime;
			float middleGray;
		};
		bool m_isEnable = false;
		static const int NUM_CALC_AVG_RT = 5;							//���ϋP�x�v�Z�p�̃����_�����O�^�[�Q�b�g�̖����B
		CRenderTargetDx12 m_calcAvgRT[NUM_CALC_AVG_RT];	//���ϋP�x�v�Z�p�̃����_�����O�^�[�Q�b�g�B
		CRenderTargetDx12 m_avgRT[2];									//���ϋP�x���i�[����郌���_�����O�^�[�Q�b�g�B
		int m_currentAvgRt = 0;
		CRootSignatureDx12 m_rootSignature;	//�g�[���}�b�v�`��p�̃��[�g�V�O�l�`���B
		CPipelineStateDx12 m_calcLuminanceLogAvaragePipelineState;	//�P�x�̑ΐ����ς����߂�Ƃ��̃p�C�v���C���X�e�[�g�B
		CPipelineStateDx12 m_calsLuminanceAvaragePipelineState;		//�P�x�̕��ς����߂�Ƃ��̃p�C�v���C���X�e�[�g�B
		CPipelineStateDx12 m_calsLuminanceExpAvaragePipelineState;	//�P�x�̎w�����ς����߂�Ƃ��̃p�C�v���C���X�e�[�g�B
		CPipelineStateDx12 m_psCalcAdaptedLuminanceFirstPipelineState;	//
		CPipelineStateDx12 m_psCalcAdaptedLuminancePipelineState;
		CPipelineStateDx12 m_finalPipelineState;
		CShaderDx12			m_vs;								//���_�V�F�[�_�[�B
		CShaderDx12			m_psCalcLuminanceLogAvarageShader;	//�P�x�̑ΐ����ς����߂�s�N�Z���V�F�[�_�[�B
		CShaderDx12			m_psCalcLuminanceAvarageShader;		//�P�x�̕��ς����߂�s�N�Z���V�F�[�_�[�B
		CShaderDx12			m_psCalcLuminanceExpAvarageShader;	//�P�x�̎w�����ς����߂�s�N�Z���V�F�[�_�[�B
		CShaderDx12			m_psCalcAdaptedLuminanceShader;		//���Ï����̃s�N�Z���V�F�[�_�[�B
		CShaderDx12			m_psCalcAdaptedLuminanceFirstShader;	//���Ï����̃s�N�Z���V�F�[�_�[�B(�V�[�����؂�ւ�����Ƃ��Ɏg�p�����B)
		CShaderDx12			m_psFinal;											//�ŏI�����V�F�[�_�[�B
		bool						m_isFirstWhenChangeScene = true;			//�V�[�����؂�ւ���ď���̕`�悩�ǂ����̃t���O�B
		CVector4				m_avSampleOffsets[MAX_SAMPLES];
		CConstantBufferDx12	m_cbCalcLuminance[NUM_CALC_AVG_RT];
		CConstantBufferDx12 m_cbTonemapCommon;
		STonemapParam	m_tonemapParam;
	};
}