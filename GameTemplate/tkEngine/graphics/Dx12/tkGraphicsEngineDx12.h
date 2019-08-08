#pragma once

#include "tkEngine/graphics/tkIGraphicsEngineImp.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>

namespace tkEngine {
	/// <summary>
	/// DirectX12に依存するグラフィックスエンジンの実装
	/// </summary>
	class CGraphicsEngineDx12 : public IGraphicsEngineImp {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="hwnd">ウィンドウハンドル</param>
		/// <param name="initParam">初期化用のパラメータ</param>
		/// <returns>falseが返ってきたら初期化に失敗。</returns>
		bool Init(HWND hwnd, const SInitParam& initParam) override final;
		/// <summary>
		/// 終了処理。アプリケーションが終了するときに呼び出してください。
		/// </summary>
		void Destroy() override final;
		/// <summary>
		/// 描画。
		/// </summary>
		void Render() override final;
	private:
		/// <summary>
		/// DXGIファクトリの作成。
		/// </summary>
		/// <returns>作成されたDXGIファクトリ。</returns>
		ComPtr<IDXGIFactory4> CreateDXGIFactory();
		/// <summary>
		/// D3Dデバイスの作成。
		/// </summary>
		/// <returns>falseが返ってきたら作成に失敗。</returns>
		bool CreateD3DDevice();
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
		void WaitForPreviousFrame();
		
	private:
		enum {
			FRAME_COUNT = 2
		};
		ComPtr<ID3D12Device>				m_d3dDevice;		//D3Dデバイス。
		ComPtr<ID3D12CommandQueue>			m_commandQueue;		//コマンドキュー。
		ComPtr< IDXGISwapChain3>			m_swapChain;		//スワップチェイン。
		ComPtr< ID3D12DescriptorHeap>		m_rtvHeap;			//フレームバッファのRTV用のディスクリプタヒープ。
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;	//コマンドアロケータ。
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;		//コマンドリスト。
		ComPtr<ID3D12PipelineState>			m_pipelineState;	//パイプラインステート。
		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;
		UINT m_rtvDescriptorSize = 0;						//フレームバッファのディスクリプタのサイズ。
		int m_currentBackBufferIndex = 0;					//現在のバックバッファの番号。
		std::array< ComPtr<ID3D12Resource>, FRAME_COUNT> m_renderTargets;	//フレームバッファ世のレンダリングターゲット。
	};
}