#pragma once

namespace tkEngine {
	/// <summary>
	/// スキンモデルの実装のインターフェースクラス。
	/// </summary>
	class IModelImp : Noncopyable {
	public:
		IModelImp();
		virtual ~IModelImp();
	};
}