#pragma once

namespace tkEngine {
	class IModelImp;
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CModel();

	private:
		std::unique_ptr<IModelImp>	m_imp;	//モデルの実装クラス。
	};
}