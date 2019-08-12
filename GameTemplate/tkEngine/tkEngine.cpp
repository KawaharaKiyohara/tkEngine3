/*!
 *@brief	エンジン
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/gameObject/tkGameObjectManager.h"
#include "tkEngine/timer/tkStopwatch.h"
#include <thread>

namespace tkEngine {
	IGraphicsEngine* g_graphicsEngine = nullptr;
	CCamera* g_camera3D = nullptr;
	CGameTime* g_gameTime = nullptr;
	std::array<CPad*, CPad::CONNECT_PAD_MAX> g_pad;

	CEngine::CEngine()
	{
		m_graphicsEngine = IGraphicsEngine::CreateInstance();
		g_graphicsEngine = m_graphicsEngine.get();
		g_camera3D = &m_graphicsEngine->GetCamera3D();
		g_gameTime = &m_gameTime;
		for (int i = 0; i < CPad::CONNECT_PAD_MAX; i++) {
			g_pad[i] = &m_pad[i];
		}
	}
	CEngine::~CEngine()
	{
		Final();
	}
	bool CEngine::Init(const SInitParam& initParam)
	{
		std::locale("");
		//カレントディレクトリをAssetsに。
		SetCurrentDirectory("Assets");
		//ウィンドウ初期化。
		if (!InitWindow(initParam)) {
			return false;
		}
		//GameObjectManagerの初期化。
		GameObjectManager().Init(initParam.gameObjectPrioMax);
		//GraphicsEngineの初期化。
		if (!m_graphicsEngine->Init(m_hWnd, initParam)) {
			return false;
		}
		//SoundEngineの初期化
		m_soundEngine.Init();
;
		//乱数を初期化。
		m_random.Init((unsigned long)time(NULL));
		//パッドを初期化。
		int padNo = 0;
		for (auto& pad : m_pad) {
			pad.Init(padNo);
			padNo++;
		}
#
		return true;
	}
	
	bool CEngine::InitWindow(const SInitParam& initParam)
	{
		TK_ASSERT(initParam.screenHeight != 0, "screenHeight is zero");
		TK_ASSERT(initParam.screenWidth != 0, "screenWidth is zero");

		m_screenHeight = initParam.screenHeight;
		m_screenWidth = initParam.screenWidth;
		WNDCLASSEX wc =
		{
			sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
			GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
			TEXT("GameDemo"), nullptr
		};
		RegisterClassEx(&wc);
		// Create the application's window
		RECT rc = { 0, 0, m_screenWidth,m_screenHeight };
		AdjustWindowRect(
			&rc,                                      // クライアント矩形
			WS_OVERLAPPED,							  // ウィンドウスタイル
			FALSE                                     // メニューフラグ
		);
		m_hWnd = CreateWindow(TEXT("GameDemo"), TEXT("GameDemo"),
			WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
			nullptr, nullptr, wc.hInstance, nullptr);


		ShowWindow(m_hWnd, initParam.nCmdShow);
		return m_hWnd != nullptr;
	}
	void CEngine::ResourceUnload()
	{
		m_soundEngine.GetWaveFileBank().ReleaseAll();
	}
	void CEngine::Final()
	{
		if (m_gameThread) {
			//ゲームスレッドを殺す。
			m_isReqDeadGameThread = true;
			m_isRunGameThreadCv.notify_all();
			m_gameThread->join();
		}

		m_soundEngine.Release();
		m_graphicsEngine->Release();
		
	}
	
	void CEngine::RunGameLoop()
	{
		MSG msg = {0};
	    while( WM_QUIT != msg.message )
	    {
	        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	        {
	            TranslateMessage( &msg );
	            DispatchMessage( &msg );
	        }
	        else
	        {
				//更新。
				Update();
	        }
	    }
	}
	
	void CEngine::GameUpdate()
	{
		//ゲームループ
		//パッドの更新。
		CPad::BeginFrame();
		for (auto& pad : m_pad) {
			pad.Update();
		}
		//サウンドエンジンの更新。
		m_soundEngine.Update();
		//GameObjectManager更新
		GameObjectManager().ExecuteFromGameThread();
	}
	void CEngine::Update()
	{
		m_sw.Start();
		//直列。
		GameUpdate();
		
		g_gameTime->PushFrameDeltaTime((float)m_sw.GetElapsed());
	}
	LRESULT CALLBACK CEngine::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch (msg)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
	}
}