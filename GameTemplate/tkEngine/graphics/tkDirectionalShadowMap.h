#pragma once

namespace tkEngine {
	class CModel;
	/// <summary>
	/// �w�����V���h�E�}�b�v�̊��N���X
	/// </summary>
	/// <remarks>
	/// �J�X�P�[�h�V���h�E�}�b�v���̗p�B
	/// </remarks>
	class IDirectionalShadowMap : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		void Init(const SShadowRenderConfig& cfg)
		{
			m_isEnable = cfg.isEnable;
			m_lightHeight = cfg.lightHeight;

			OnInit(cfg);
		}
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�B
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(IRenderContext& rc)
		{
			if (m_isEnable == false) {
				return;
			}
			//�����_�����O�X�e�b�v���V���h�E�}�b�v�쐬�ɕύX����B
			rc.SetRenderStep(enRenderStep_CreateDirectionalShadowMap);
			OnRenderToShadowMap(rc);
			m_shadowCasters.clear();
		}
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�̊����҂��B
		/// </summary>
		/// <remarks>
		/// �����_�����O�̊����҂����K�v�ȃv���b�g�t�H�[���Ŏ������Ă��������B
		/// </remarks>
		/// <param name="rc"></param>
		virtual void WaitEndRenderToShadowMap(IRenderContext& rc) = 0;
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();
		/// <summary>
		/// �V���h�E�}�b�v��L���ɂ���B
		/// </summary>
		void SetEnable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// �V���h�E�}�b�v�𖳌��ɂ���B
		/// </summary>
		void SetDisable()
		{
			m_isEnable = false;
		}
		/// <summary>
		/// �V���h�E�}�b�v�փ����_�����O����I�u�W�F�N�g�o�^�B
		/// </summary>
		/// <param name="rc"></param>
		void Entry(CModel* caster) {
			if (!m_isEnable) {
				return;
			}
			m_shadowCasters.push_back(caster);
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="caster">�L���X�^�[</param>
		void Remove(CModel* caster)
		{
			auto it = std::find(m_shadowCasters.begin(), m_shadowCasters.end(), caster);
			if (it != m_shadowCasters.end()) {
				//�������̂ō폜�B
				m_shadowCasters.erase(it);
			}
		}
	private:
		/// <summary>
		/// ���C�g�̍��W���v�Z����B
		/// </summary>
		/// <remarks>
		/// �������ꂽ��������ʂ����C�g�̍��W���v�Z���܂��B
		/// ���C�g�̍��W�̌v�Z�̓��C�g�̍����A���C�g�̕����A�����āA
		/// ������̒��S���W(�ߕ��ʂ̒��S���W�Ɖ����ʂ̒��S���W�����񂾐����̒��_���W)
		/// ����v�Z����܂��B
		/// �����̃p�����[�^�����ƂɃ��C�g�̍��W�͉��L�̌v�Z�ŋ��߂��܂��B
		/// 
		/// �@�@���m�����ɂ��Ă̈ꎟ�������������܂��B
		/// ���C�g�̍��� = ������̒��S���W.y + ���C�g�̕���.y * ��
		/// �� = ( ���C�g�̍��� - ������̒��S���W.y ) / ���C�g�̕���.y
		/// �A�@�����g���āA���C�g�̍��W�����߂�B
		/// ���C�g�̍��W = ������̒��S���W + ���C�g�̕��� * ��
		/// </remarks>
		/// <param name="lightHeight">
		/// ���C�g�̍����B
		/// </param>
		/// <param name="viewFrustomCenterPosition">
		/// �������ꂽ������̒��S���W�B
		/// </param>
		/// <returns>�v�Z���ꂽ���C�g�̍��W</returns>
		CVector3 CalcLightPosition(float lightHeight, CVector3 viewFrustomCenterPosition);
		/// <summary>
		/// ���������ɌĂ΂�鏈���B
		/// </summary>
		virtual void OnInit(const SShadowRenderConfig& cfg) = 0;
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O���ɌĂ΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRenderToShadowMap(IRenderContext& rc) = 0;
	protected:
		/// <summary>
		/// ���̒��g��ύX������AmodelCB.h��ShadowCb���ύX����悤�ɁB
		/// </summary>
		struct SShadowCb {
			CMatrix mLVP[NUM_SHADOW_MAP];
			CVector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
			float pading;
			float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//�J������Ԃł̉e�𗎂Ƃ��G���A�̐[�x�e�[�u���B
		};
		enum { NUN_SHADOW_MAP = 3 };					//�V���h�E�}�b�v�̐��B
		std::vector<CModel*>	m_shadowCasters;		//�V���h�E�L���X�^�[�̃��X�g�B
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP];			//���C�g�r���[�v���W�F�N�V�����s��B
		SShadowCb m_shadowCbEntity;
	private:
		
		CVector3 m_lightDirection = CVector3::Down;						//���C�g�̕����B
		float m_shadowAreas[NUM_SHADOW_MAP] = { 0.0f };	//�e��������͈́B
		float m_lightHeight = 100.0f;					//���C�g�̍����B
		bool m_isEnable = false;						//�V���h�E�}�b�v���L�����ǂ����B
	};

}