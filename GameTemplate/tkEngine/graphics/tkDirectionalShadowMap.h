#pragma once

namespace tkEngine {
	/// <summary>
	/// �w�����V���h�E�}�b�v�̊��N���X
	/// </summary>
	/// <remarks>
	/// �J�X�P�[�h�V���h�E�}�b�v���̗p�B
	/// </remarks>
	class CDirectionalShadowMap : Noncopyable {
	public:
		/// <summary>
		/// �V���h�E�}�b�v�ւ̃����_�����O�J�n���ɌĂ΂�鏈���B
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnBeginRenderToShadowMap(IRenderContext& rc) = 0;
	private:

	};
}