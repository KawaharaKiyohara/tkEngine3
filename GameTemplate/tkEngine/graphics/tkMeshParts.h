/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once

namespace tkEngine {
	class CTkmFile;
	/// <summary>
	/// メッシュパーツのインターフェース。
	/// </summary>
	class IMeshParts : public Noncopyable {
	public:
		IMeshParts(const CTkmFile& tkmFile) {}
		virtual ~IMeshParts() {}
		/// <summary>
		/// 派生クラスに型変換。
		/// 
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
	};
}