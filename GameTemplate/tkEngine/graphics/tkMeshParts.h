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
		IMeshParts(const CTkmFile& tkmFile) {}
		virtual ~IMeshParts() {}
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