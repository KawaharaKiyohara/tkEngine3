#pragma once


#include "tkEngine/graphics/tkDirectionalShadowMap.h"

namespace tkEngine {
	class CModel;
	class IRenderContext;
	/// <summary>
	/// �w�����V���h�E�}�b�v�̊��N���X
	/// </summary>
	/// <remarks>
	/// �J�X�P�[�h�V���h�E�}�b�v���̗p�B
	/// </remarks>
	class CDirectionalShadowMapDx12 : public IDirectionalShadowMap {
	public:
		
	private:
		/// <summary>
		/// �����������B
		/// </summary>
		void OnInit(const SShadowRenderConfig& cfg) override final;
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�����B
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderToShadowMap(IRenderContext& rc) override final;
	private:
		enum { NUN_SHADOW_MAP = 3 };								//�V���h�E�}�b�v�̐��B
		std::vector<CModel*>	m_shadowCasters;					//�V���h�E�L���X�^�[�̃��X�g�B
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { g_matIdentity };	//���C�g�r���[�v���W�F�N�V�����s��B
		CRenderTargetDx12 m_shadowMaps[NUM_SHADOW_MAP];				//�V���h�E�}�b�v�B
	};
}