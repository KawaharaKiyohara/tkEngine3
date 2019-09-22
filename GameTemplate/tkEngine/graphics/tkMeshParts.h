/// <summary>
/// ���b�V���p�[�c�N���X�B
/// </summary>

#pragma once

namespace tkEngine {
	class CTkmFile;
	/// <summary>
	/// ���b�V���p�[�c�̃C���^�[�t�F�[�X�B
	/// </summary>
	class IMeshParts : public Noncopyable {
	public:

		/// <summary>
		/// tkm�t�@�C�����珉����
		/// </summary>
		/// <param name="tkmFile">tkm�t�@�C���B</param>
		virtual void InitFromTkmFile(const CTkmFile& tkmFile) = 0;
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mWorld">���[���h�s��</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mWorld, const CMatrix& mView, const CMatrix& mProj) = 0;
		/// <summary>
		/// �h���N���X�Ɍ^�ϊ��B
		/// 
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
	};
}