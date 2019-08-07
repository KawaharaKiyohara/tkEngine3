#pragma once

namespace tkEngine {
	/// <summary>
	/// スキンモデルの実装のインターフェースクラス。
	/// </summary>
	class ISkinModelImp : Noncopyable {
	public:
		ISkinModelImp();
		virtual ~ISkinModelImp();
	};
}