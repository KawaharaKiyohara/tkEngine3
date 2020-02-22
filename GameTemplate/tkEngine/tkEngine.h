/*!
 *@file	tkEngine.h
 */
#pragma once

#include "tkEngine/timer/tkStopwatch.h"
#include "tkEngine/Sound/tkSoundEngine.h"
#include "tkEngine/tkEngineStruct.h"
#include "tkEngine/random/tkRandom.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/graphics/tkGraphicsInstanceFactory.h"

 /*!
  *@namespace	tkEngine全体の名前空間。
  */
namespace tkEngine{
	/*!
	 *@brief	河原学園内製ゲームエンジン version 3
	 */
	class CEngine : Noncopyable {
	private:
		/*!
		 *@brief	コンストラクタ。
		 */
		CEngine();
		/*!
		 *@brief	デストラクタ。
		 */
		~CEngine();
	public:
		/*!
		 *@brief	初期化。
		 *@param[in]	initParam		初期化パラメータ。
		 */
		static bool Init(const SInitParam& initParam);
		/*!
		 *@brief	終了処理。
		 */
		void Final();
		/*!
		 *@brief	ゲームループを実行。
		 */
		void RunGameLoop();
		/*!
		 *@brief	インスタンスを取得。
		 */
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;
			}
			return *instance;
		}

		/*!
		* @brief	ゲームパッドの取得。
		*@param[in]	padNo	パッド番号。CPad::CONNECT_PAD_MAX-1まで指定可能。
		*/
		CPad& GetPad(int padNo)
		{
			TK_ASSERT(padNo < CPad::CONNECT_PAD_MAX && padNo >= 0, "padNo is invalid");
			return m_pad[padNo];
		}
		/*!
		* @brief	サウンドエンジンの取得。
		*/
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
		
		/*!
		 *@brief	乱数取得。
		 */
		CRandom& GetRandom()
		{
			return m_random;
		}
		/*!
		*@brief		リソースのアンロード。
		*@details
		* 常駐リソース以外のリソースをすべてアンロードします。
		* シーンの切り替えなどで、リソースのアンロードが必要な場合に使用してください。
		*/
		void ResourceUnload();

		/// <summary>
		/// グラフィックスインスタンスのファクトリを取得。
		/// </summary>
		/// <returns></returns>
		IGraphicsInstanceFactory* GetGraphicsInstanceFactory()
		{
			return m_graphisInstanceFactory.get();
		}
	private:
		/// <summary>
		/// 内部の初期化処理
		/// </summary>
		/// <param name="initParam">初期化パラメータ。</param>
		/// <returns></returns>
		bool InitInternal(const SInitParam& initParam);
		/*!
		 *@brief	ウィンドウ初期化。
		 */
		bool InitWindow( const SInitParam& initParam );
		
		/// <summary>
		/// ゲームスレッド。
		/// </summary>
		void GameThread();
		/*!
		* @brief	ウィンドウプロシージャ。
		*@param[in]	hWnd	ウィンドウハンドル
		*@param[in] msg		メッセージ
		*@param[in] wParam	WPARAM
		*@param[in] lParam	LPARAM
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		/*!
		* @brief	更新。
		*/
		void Update();
		/// <summary>
		/// ゲーム部分の更新処理。
		/// </summary>
		void GameUpdate();
	private:
		HINSTANCE				m_hInst = nullptr;	//!<アプリケーションのインスタンス。
		HWND					m_hWnd = nullptr;	//!<ウィンドウハンドル。
		CSoundEngine			m_soundEngine;								//!<サウンドエンジン。
		UPIGraphicsInstanceFactory m_graphisInstanceFactory;				//グラフィックスインスタンスのファクトリ。
		UPIGraphicsEngine		m_graphicsEngine;							//グラフィックエンジン。
		CGameTime				m_gameTime;									//ゲームタイム。
		CPhysicsWorld			m_physicsWorld;		//物理ワールド。
		int						m_screenWidth = 0;							//!<スクリーンの幅。
		int						m_screenHeight = 0;							//!<スクリーンの高さ。
		CPad					m_pad[CPad::CONNECT_PAD_MAX];				//!<ゲームパッド。
		CRandom					m_random;									//!<乱数。
		std::unique_ptr<std::thread> m_gameThread;							//ゲームスレッド。
		bool			m_isRunGameThread = false;
		bool			m_isReqDeadGameThread = false;
		std::mutex		m_isRunGameThreadMtx;
		std::condition_variable m_isRunGameThreadCv;
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		float					m_fps = 30.0f;
		float					m_timeTotal = 0.0f;
#endif
	public:
		CStopwatch				m_sw;
	};
	//エンジンの外部にインスタンスを公開するグローバルなアクセスポイント。
	extern CEngine* g_engine;
	extern IGraphicsEngine* g_graphicsEngine;
	extern CCamera* g_camera3D;
	extern CCamera* g_camera2D;
	extern CGameTime* g_gameTime;
	extern ILightManager* g_lightManager;
	extern std::array<CPad*, CPad::CONNECT_PAD_MAX> g_pad;
	extern CPhysicsWorld* g_physicsWorld;

	/*!
	* @brief	CSoundEngineのインスタンスを取得。
	*/
	static inline CSoundEngine& SoundEngine()
	{
		return g_engine->GetSoundEngine();
	}

	/*!
	 *@brief	乱数を取得。
	 */
	static inline CRandom& Random()
	{
		return g_engine->GetRandom();
	}
	
}


