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
		virtual bool Init(HWND hwnd, const SInitParam& initParam) = 0;
	};
}