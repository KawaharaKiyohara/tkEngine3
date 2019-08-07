#pragma once

namespace tkEngine {
	class ISkinModelImp;
	/// <summary>
	/// スキンモデルクラス。
	/// </summary>
	class CSkinModel : Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CSkinModel();

	private:
		std::unique_ptr<ISkinModelImp>	m_imp;	//スキンモデルの実装クラス。
	};
}