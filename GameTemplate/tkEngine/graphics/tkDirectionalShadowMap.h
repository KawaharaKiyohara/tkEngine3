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
			OnInit(cfg);
		}
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�B
		/// </summary>
		/// <param name="rc"></param>
		virtual void RenderToShadowMap(IRenderContext& rc) = 0;
		/// <summary>
		/// �X�V�����B
		/// </summary>
		void Update();
	private:
		/// <summary>
		/// ���������ɌĂ΂�鏈���B
		/// </summary>
		virtual void OnInit(const SShadowRenderConfig& cfg) = 0;
	private:
		enum { NUN_SHADOW_MAP = 3 };								//�V���h�E�}�b�v�̐��B
		std::vector<CModel*>	m_shadowCasters;					//�V���h�E�L���X�^�[�̃��X�g�B
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { g_matIdentity };	//���C�g�r���[�v���W�F�N�V�����s��B
	};

}