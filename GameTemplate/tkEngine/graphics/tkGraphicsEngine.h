#pragma once

#include "tkCamera.h"

namespace tkEngine {
	
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
		/// 毎フレーム呼ばれる更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="goMgr">ゲームオブジェクトマネージャ</param>
		void Render(CGameObjectManager* goMgr);
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
		/// 2Dカメラの取得。
		/// </summary>
		/// <returns></returns>
		CCamera& GetCamera2D()
		{
			return m_camera2D;
		}
		/// <summary>
		/// ライトマネージャーを取得。
		/// </summary>
		/// <returns></returns>
		UPILightManager& GetLightManager() 
		{
			return m_lightManager;
		}
		/// <summary>
		/// ディレクショナルシャドウマップを取得。
		/// </summary>
		/// <returns></returns>
		UPIDirectionalShadowMap& GetDirectionalShadowMap()
		{
			return m_directionalShadowMap;
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
		/// プリレンダリング。
		/// </summary>
		void PreRender();
		/// <summary>
		/// 初期化時に呼ばれる処理。
		/// </summary>
		/// <param name="hwnd"></param>
		/// <param name="initParam"></param>
		/// <returns></returns>
		virtual bool OnInit(HWND hwnd, const SInitParam& initParam) = 0;
		/// <summary>
		/// 描画時に呼ばれる処理。
		/// </summary>
		/// <param name="goMgr">ゲームオブジェクトマネージャ</param>
		virtual void OnRender(CGameObjectManager* goMgr) = 0;
		/// <summary>
		/// レンダリング開始時に呼ばれる処理。
		/// </summary>
		virtual void OnBeginRender() = 0;
		/// <summary>
		/// レンダリング終了時に呼ばれる処理。
		/// </summary>
		virtual void OnEndRender() = 0;
		/// <summary>
		/// プリレンダリング開始時に呼ばれる処理。
		/// </summary>
		virtual void OnBeginPreRender() = 0;
		/// <summary>
		/// プリレンダリング終了時に呼ばれる処理。
		/// </summary>
		virtual void OnEndPreRender() = 0;
	protected:
		int	m_frameBufferWidth = 0;			//フレームバッファの幅。
		int m_frameBufferHeight = 0;		//フレームバッファの高さ。
		const int m_2dSpaceScreenWidth = 1280;	//2D空間のスクリーンの幅。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		const int m_2dSpaceScreenHeight = 720;	//2D空間のスクリーンの高さ。2Dはこの座標系を前提に表示されていれば、解像度が変わっても大丈夫。
		CCamera m_camera3D;					//3Dカメラ。	
		CCamera m_camera2D;					//2Dカメラ。
		CCamera m_cameraPostEffect;			//ポストエフェクト用のカメラ。
		UPIRenderContext m_renderContext;	//レンダリングコンテキスト。今は一本。
		UPILightManager m_lightManager;		//ライトマネージャ。
		UPIDirectionalShadowMap m_directionalShadowMap;	//指向性シャドウマップ。
	};
}
