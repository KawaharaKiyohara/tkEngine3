#pragma once

namespace tkEngine {
	class ISkinModelImp;
	/// <summary>
	/// �X�L�����f���N���X�B
	/// </summary>
	class CSkinModel : Noncopyable {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CSkinModel();

	private:
		std::unique_ptr<ISkinModelImp>	m_imp;	//�X�L�����f���̎����N���X�B
	};
}