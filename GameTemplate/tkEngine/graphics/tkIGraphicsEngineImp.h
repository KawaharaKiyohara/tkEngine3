#pragma once

namespace tkEngine {
	/// <summary>
	/// グラフィックエンジンの実装のインターフェースクラス。
	/// </summary>
	/// <remarks>
	/// DirectXのバージョンに依存するコードを隠蔽するためのインターフェースクラスです。
	/// 
	/// </remarks>
	class IGraphicsEngineImp {
	public:
		virtual ~IGraphicsEngineImp() {};
		virtual bool Init(HWND hwnd, const SInitParam& initParam) = 0;
	};
}