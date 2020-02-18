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
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�̊����҂��B
		/// </summary>
		/// <param name="rc"></param>
		void WaitEndRenderToShadowMap(IRenderContext& rc) override final;
	private:
		CConstantBufferDx12 m_shadowCb;
		CRenderTargetDx12 m_shadowMaps[NUM_SHADOW_MAP];				//�V���h�E�}�b�v�B
	};
}