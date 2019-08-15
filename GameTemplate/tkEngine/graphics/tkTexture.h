#pragma once

namespace tkEngine {
	class ITexture : Noncopyable {
	public:
		/// <summary>
		/// ���p�\�H
		/// </summary>
		/// <returns>true���Ԃ��Ă����痘�p�\�B</returns>
		bool IsValid() const
		{
			return m_isValid;
		}
	protected:
		/// <summary>
		/// �e�N�X�`���𗘗p�\�ɂ���B
		/// </summary>
		void SetValid()
		{
			m_isValid = true;
		}
	private:
		bool m_isValid = false;	//���p�\�H
	};
}