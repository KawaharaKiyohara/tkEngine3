#pragma once

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkRenderContextDx12.h"
#include "tkBloomDx12.h"

namespace tkEngine {
	/// <summary>
	/// �|�X�g�G�t�F�N�g�N���X�B
	/// </summary>
	class CPostEffectDx12 : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="config">�O���t�B�b�N�ݒ�</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc"></param>
		void Render(IRenderContext& rc);
	
	private:
		CBloomDx12 m_bloom;	//�u���[���B
		CPrimitive m_quadPrimitive;			//�l�p�`�v���~�e�B�u�B

	};
}