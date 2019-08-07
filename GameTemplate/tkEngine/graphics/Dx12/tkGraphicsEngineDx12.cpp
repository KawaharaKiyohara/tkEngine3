#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"


namespace tkEngine {
	bool CGraphicsEngineDx12::Init(HWND hwnd, const SInitParam& initParam)
	{

		//DXGIファクトリーの作成。
		auto dxGiFactory = ComPtr<IDXGIFactory4>();
		if (dxGiFactory == false) {
			//ファクトリの作成に失敗した。
			return false;
		}
		//D3Dデバイスの作成。
		if (CreateD3DDevice() == false) {
			//デバイスの作成に失敗した。
			return false;
		}

		//コマンドキューの作成。
		if (CreateCommandQueue() == false) {
			//コマンドキューの作成に失敗した。
			return false;
		}
		return true;
	}
	ComPtr<IDXGIFactory4> CGraphicsEngineDx12::CreateDXGIFactory()
	{
		UINT dxgiFactoryFlags = 0;
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		//デバッグコントローラーがあれば、デバッグレイヤーがあるDXGIを作成する。
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif
		ComPtr<IDXGIFactory4> factory;
		CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
		return factory;
	}
	bool CGraphicsEngineDx12::CreateD3DDevice()
	{
		auto hr = D3D12CreateDevice(
			nullptr,
			D3D_FEATURE_LEVEL_12_1,
			IID_PPV_ARGS(&m_d3dDevice)
		);
		if (FAILED(hr)) {
			//D3Dデバイスの作成に失敗した。
			return false;
		}
		return true;
	}
	bool CGraphicsEngineDx12::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		auto hr = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
		if (FAILED(hr)) {
			//コマンドキューの作成に失敗した。
			return false;
		}
		return true;
	}
	bool CGraphicsEngineDx12::CreateSwapChain(
		HWND hwnd, 
		const SInitParam& initParam, 
		ComPtr<IDXGIFactory4> dxgiFactory )
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.Width = initParam.frameBufferWidth;
		swapChainDesc.Height = initParam.frameBufferHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapChain;
		dxgiFactory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&swapChain
		);
		//IDXGISwapChain3のインターフェースを取得。
		swapChain.As(&m_swapChain);
		m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
		return true;
	}
}