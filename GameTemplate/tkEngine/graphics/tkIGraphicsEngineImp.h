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
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化パラメータ</param>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		virtual bool Init(HWND hwnd, const SInitParam& initParam) = 0;
		/// <summary>
		/// 終了処理。アプリケーションが終了するときに呼び出してください。
		/// </summary>
		virtual void Destroy() = 0;
		/// <summary>
		/// 描画。
		/// </summary>
		virtual void Render(
			std::function<void()> onRender,
			std::function<void()> onPreForwardRender,
			std::function<void()> onForwardRender,
			std::function<void()> onPostRender
		) = 0;
	};
}