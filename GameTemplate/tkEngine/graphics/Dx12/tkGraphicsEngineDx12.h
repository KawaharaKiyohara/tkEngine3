#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkGraphicsEngine.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include "tkRenderTargetDx12.h"
#include "tkSpriteDx12.h"

#include "postEffect/tkPostEffectDx12.h"

namespace tkEngine {
	/// <summary>
	/// DirectX12に依存するグラフィックスエンジン
	/// </summary>
	class CGraphicsEngineDx12 : public IGraphicsEngine {
	public:
		enum {
			FRAME_COUNT = 2
		};
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化用のパラメータ</param>
		/// <returns>falseが返ってきたら初期化に失敗。</returns>
		bool OnInit(HWND hwnd, const SInitParam& initParam) override final;
		/// <summary>
		/// 終了処理。アプリケーションが終了するときに呼び出してください。
		/// </summary>
		void Destroy() override final;
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="goMgr">ゲームオブジェクトマネージャ</param>
		void OnRender(CGameObjectManager* goMgr) override final;
		/// <summary>
		/// D3Dデバイスを取得。
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12Device> GetD3DDevice() const
		{
			return m_d3dDevice;
		}
		/// <summary>
		/// コマンドキューを取得。
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const
		{
			return m_commandQueue;
		}
		/// <summary>
		/// バックバッファの番号を取得。
		/// </summary>
		/// <returns>バックバッファの番号。</returns>
		UINT GetBackBufferIndex() const
		{
			return m_frameIndex;
		}
		/// <summary>
		/// CBR_SRVのディスクリプタのサイズを取得。
		/// </summary>
		/// <returns></returns>
		UINT GetCbrSrvDescriptorSize() const
		{
			return m_cbrSrvDescriptorSize;
		}
		/// <summary>
		/// メインレンダリングターゲットを取得。
		/// </summary>
		/// <returns></returns>
		CRenderTargetDx12& GetMainRenderTarget()
		{
			return m_mainRenderTarget;
		}
		/// <summary>
		/// メインレンダリングターゲットのビューポートを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_VIEWPORT& GetMainRenderTargetViewport()
		{
			return m_viewport;
		}

	private:
		/// <summary>
		/// 1フレーム描画開始時の処理。
		/// </summary>
		void OnBeginRender() override final;
		/// <summary>
		/// 1フレームの描画終了時の処理。
		/// </summary>
		void OnEndRender() override final;
		/// <summary>
		/// プリレンダリング開始時に呼ばれる処理。
		/// </summary>
		void OnBeginPreRender()override final;
		/// <summary>
		/// プリレンダリング終了時に呼ばれる処理。
		/// </summary>
		void OnEndPreRender() override final;
		/// <summary>
		/// DXGIファクトリの作成。
		/// </summary>
		/// <returns>作成されたDXGIファクトリ。</returns>
		ComPtr<IDXGIFactory4> CreateDXGIFactory();
		/// <summary>
		/// D3Dデバイスの作成。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateD3DDevice(ComPtr<IDXGIFactory4> dxgiFactory);
		/// <summary>
		/// コマンドキューの作成。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateCommandQueue();
		/// <summary>
		/// スワップチェインの作成。
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化パラメータ。</param>
		/// <param name="dxgiFactory">DXGIファクトリー。</param>
		/// <returns>falseが返ったきたら作成に失敗。</returns>
		bool CreateSwapChain(HWND hwnd, const SInitParam& initParam, ComPtr<IDXGIFactory4> dxgiFactory);
		/// <summary>
		/// フレームバッファのためのディスクリプタヒープを作成する。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateDescriptorHeapForFrameBuffer();
		/// <summary>
		/// フレームバッファのためのレンダリングターゲットビューを作成する。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateRTVForFameBuffer();
		/// <summary>
		/// フレームバッファのための深度ステンシルビューを作成する。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateDSVForFrameBuffer(const SInitParam& initParam);
		/// <summary>
		/// コマンドリストの作成。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateCommandList();
		/// <summary>
		/// GPUと同期をとるオブジェクトを作成する。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateSynchronizationWithGPUObject();
		/// <summary>
		/// 1フレーム前の描画が終わるのを待つ。
		/// </summary>
		void WaitDraw();
	private:
		D3D12_VIEWPORT						m_viewport;				//ビューポート。
		D3D12_RECT							m_scissorRect;			//シザリング矩形。
		ComPtr<ID3D12Device>				m_d3dDevice;			//D3Dデバイス。
		ComPtr<ID3D12CommandQueue>			m_commandQueue;			//コマンドキュー。
		ComPtr< IDXGISwapChain3>			m_swapChain;			//スワップチェイン。
		ComPtr< ID3D12DescriptorHeap>		m_rtvHeap;				//フレームバッファのRTV用のディスクリプタヒープ。
		ComPtr< ID3D12DescriptorHeap>		m_dsvHeap;				//フレームバッファ用の深度ステンシルバッファビューのディスクリプタヒープ。
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;		//コマンドアロケータ。
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;			//コマンドリスト。
		ComPtr<ID3D12PipelineState>			m_pipelineState;		//パイプラインステート。
		CRenderTargetDx12					m_mainRenderTarget;		//メインレンダリングターゲット。
		CSpriteDx12 m_copyFullScreenSprite;	//フルスクリーンコピー用のスプライト。
		CPostEffectDx12 m_postEffect;
		
		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;
		UINT m_rtvDescriptorSize = 0;						//フレームバッファのディスクリプタのサイズ。
		UINT m_dsvDescriptorSize = 0;						//深度ステンシルバッファのディスクリプタのサイズ。
		UINT m_cbrSrvDescriptorSize = 0;					//CBR_SRVのディスクリプタのサイズ。
		int m_currentBackBufferIndex = 0;					//現在のバックバッファの番号。
		std::array< ComPtr<ID3D12Resource>, FRAME_COUNT> m_renderTargets;	//フレームバッファ用のレンダリングターゲット。
		ComPtr<ID3D12Resource> m_depthStencilBuffer;	//深度ステンシルバッファ。
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12