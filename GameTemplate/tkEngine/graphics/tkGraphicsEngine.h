#pragma once

#include "tkIGraphicsEngineImp.h"
#include "tkCamera.h"

namespace tkEngine {
	class CGameObjectManager;
	/// <summary>
	/// グラフィックエンジン
	/// </summary>
	class CGraphicsEngine : Noncopyable{
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
		void Render(
			std::function<void()> onRender,
			std::function<void()> onPreForwardRender, 
			std::function<void()> onForwardRender,
			std::function<void()> onPostRender
		);
		/// <summary>
		/// 破棄。
		/// </summary>
		void Destroy();
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
		/// CGraphicsEngineの実装を取得。
		/// </summary>
		template<class T>
		T* GetImplement()
		{
			return dynamic_cast<T*>(m_imp.get());
		}
	private:
		int	m_frameBufferWidth = 0;		//フレームバッファの幅。
		int m_frameBufferHeight = 0;	//フレームバッファの高さ。
		CCamera m_camera3D;				//3Dカメラ。	
		std::unique_ptr< IGraphicsEngineImp> m_imp;	//DirectXのバージョンに依存するエンジンの実装。
	};
}