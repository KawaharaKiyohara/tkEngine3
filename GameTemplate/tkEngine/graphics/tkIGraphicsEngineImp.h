#pragma once

namespace tkEngine {
	/// <summary>
	/// �O���t�B�b�N�G���W���̎����̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	/// <remarks>
	/// DirectX�̃o�[�W�����Ɉˑ�����R�[�h���B�����邽�߂̃C���^�[�t�F�[�X�N���X�ł��B
	/// 
	/// </remarks>
	class IGraphicsEngineImp {
	public:
		virtual ~IGraphicsEngineImp() {};
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�����[�^</param>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		virtual bool Init(HWND hwnd, const SInitParam& initParam) = 0;
		/// <summary>
		/// �I�������B�A�v���P�[�V�������I������Ƃ��ɌĂяo���Ă��������B
		/// </summary>
		virtual void Destroy() = 0;
		/// <summary>
		/// �`��B
		/// </summary>
		virtual void Render(
			std::function<void()> onRender,
			std::function<void()> onPreForwardRender,
			std::function<void()> onForwardRender,
			std::function<void()> onPostRender
		) = 0;
	};
}