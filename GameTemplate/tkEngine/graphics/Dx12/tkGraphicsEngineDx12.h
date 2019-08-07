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
	private:
		ComPtr<ID3D12Device>		m_d3dDevice;	//D3Dデバイス。
		ComPtr<ID3D12CommandQueue>	m_commandQueue;	//コマンドキュー。
		ComPtr< IDXGISwapChain3>	m_swapChain;	//スワップチェイン。
		int m_currentBackBufferIndex = 0;			//現在のバックバッファの番号。
	};
}