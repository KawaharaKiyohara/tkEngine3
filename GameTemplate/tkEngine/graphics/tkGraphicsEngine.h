#pragma once

#include "tkCamera.h"

namespace tkEngine {
	//外部に公開するユニークポインタ色々。
	
	class CGameObjectManager;
	
	/// <summary>
	/// グラフィックエンジン
	/// </summary>
	class IGraphicsEngine : Noncopyable{
	public:
		/// <summary>
		/// グラフィックエンジンを初期化。
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化パラメータ</param>
		/// <returns>falseが返ってきたら初期化に失敗。</returns>
		bool Init(HWND hwnd, const SInitParam& initParam);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="onRender">G-Bufferへのレンダリングパスで呼ばれる関数。</param>
		/// <param name="onPreForwardRender">プレフォワードレンダリングのパスで呼ばれる関数。</param>
		/// <param name="onForwardRender">フォワードレンダリングのパスで呼ばれる関数。</param>
		/// <param name="onPostRender">ポストレンダリングのパスで呼ばれる関数。</param>
		virtual void Render(
			std::function<void(IRenderContext& rc)> onRender,
			std::function<void(IRenderContext& rc)> onPreForwardRender,
			std::function<void(IRenderContext& rc)> onForwardRender,
			std::function<void(IRenderContext& rc)> onPostRender
		) = 0;
		/// <summary>
		/// 破棄。
		/// </summary>
		virtual void Destroy() = 0;
		/// <summary>
		/// フレームバッファの幅を取得。
		/// </summary>
		/// <returns>フレームバッファの幅。</returns>
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/// <summary>
		/// フレームバッファの高さを取得。
		/// </summary>
		/// <returns>フレームバッファの高さ。</returns>
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/// <summary>
		/// グラフィックスリソースの明示的な解放。
		/// </summary>
		void Release()
		{
		}
		/// <summary>
		/// 3Dカメラを取得。
		/// </summary>
		/// <returns></returns>
		CCamera& GetCamera3D() 
		{
			return m_camera3D;
		}
		/// <summary>
		/// CGraphicsEngineのインスタンスの型変換。
		/// </summary>
		template<class T>
		T& As()
		{
			return dynamic_cast<T&>(*this);
		}
	private:
		/// <summary>
		/// 初期化時に呼ばれる処理。
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="initParam"></param>
		/// <returns></returns>
		virtual bool OnInit(HWND hwnd, const SInitParam& initParam) = 0;
	protected:
		int	m_frameBufferWidth = 0;			//フレームバッファの幅。
		int m_frameBufferHeight = 0;		//フレームバッファの高さ。
		CCamera m_camera3D;					//3Dカメラ。	
		UPIRenderContext m_renderContext;	//レンダリングコンテキスト。今は一本。
	};
}
