#pragma once

namespace tkEngine {
	class IModelImp;
	/// <summary>
	/// ���f���N���X�B
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CModel();

	private:
		std::unique_ptr<IModelImp>	m_imp;	//���f���̎����N���X�B
	};
}