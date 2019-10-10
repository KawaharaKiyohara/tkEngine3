#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"

namespace tkEngine {
	
	void CGraphicsEngineDx12::WaitDraw()
	{
		//描画終了待ち
		// Signal and increment the fence value.
		const UINT64 fence = m_fenceValue;
		m_commandQueue->Signal(m_fence.Get(), fence);
		m_fenceValue++;

		// Wait until the previous frame is finished.
		if (m_fence->GetCompletedValue() < fence)
		{
			m_fence->SetEventOnCompletion(fence, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}

		m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	}
	void CGraphicsEngineDx12::Destroy()
	{
		WaitDraw();
		CloseHandle(m_fenceEvent);
	}
	bool CGraphicsEngineDx12::OnInit(HWND hwnd, const SInitParam& initParam)
	{
		//DXGIファクトリーの作成。
		auto dxGiFactory = CreateDXGIFactory();
		if (!dxGiFactory ) {
			//ファクトリの作成に失敗した。
			return false;
		}
		//D3Dデバイスの作成。
		if (!CreateD3DDevice()) {
			//デバイスの作成に失敗した。
			TK_WARNING_MESSAGE_BOX("D3Dデバイスの作成に失敗しました。");
			return false;
		}

		//コマンドキューの作成。
		if (!CreateCommandQueue()) {
			//コマンドキューの作成に失敗した。
			TK_WARNING_MESSAGE_BOX("コマンドキューの作成に失敗しました。");
			return false;
		}
		//スワップチェインの作成。
		if (!CreateSwapChain(hwnd, initParam, dxGiFactory)) {
			//スワップチェインの作成に失敗した。
			TK_WARNING_MESSAGE_BOX("スワップチェインの作成に失敗しました。");
			return false;
		}
		//フレームバッファのRTV用のディスクリプタヒープを作成する。
		if (!CreateDescriptorHeapForFrameBuffer()) {
			TK_WARNING_MESSAGE_BOX("ディスクリプタヒープの作成に失敗しました。");
			return false;
		}
		//フレームバッファ用のRTVを作成する。
		if (!CreateRTVForFameBuffer()) {
			TK_WARNING_MESSAGE_BOX("フレームバッファ用のRTVの作成に失敗しました。");
			return false;
		}
		//フレームバッファ用のDSVを作成する。
		if (!CreateDSVForFrameBuffer(initParam)) {
			TK_WARNING_MESSAGE_BOX("フレームバッファ用のDSVの作成に失敗しました。");
			return false;
		}
		//コマンドアロケータの作成。
		m_d3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(&m_commandAllocator));
		if (!m_commandAllocator) {
			TK_WARNING_MESSAGE_BOX("コマンドアロケータの作成に失敗しました。");
			return false;
		}
		
		//コマンドリストの作成。
		if (!CreateCommandList()) {
			TK_WARNING_MESSAGE_BOX("コマンドリストの作成に失敗しました。");
			return false;
		}
		//GPUと同期をとるためのオブジェクトを作成する。
		if (!CreateSynchronizationWithGPUObject()) {
			TK_WARNING_MESSAGE_BOX("GPUとの同期オブジェクトの作成に失敗しました。");
			return false;
		}
		//ビューポートを初期化。
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = static_cast<float>(initParam.frameBufferWidth);
		m_viewport.Height = static_cast<float>(initParam.frameBufferHeight);
		m_viewport.MinDepth = D3D12_MIN_DEPTH;
		m_viewport.MaxDepth = D3D12_MAX_DEPTH;

		//シザリング矩形を初期化。
		m_scissorRect.left = 0;
		m_scissorRect.top = 0;
		m_scissorRect.right = initParam.frameBufferWidth;
		m_scissorRect.bottom = initParam.frameBufferHeight;

		//レンダリングコンテキストの作成。
		auto giFactry = Engine().GetGraphicsInstanceFactory();
		m_renderContext = giFactry->CreateRenderContext();
		auto& rcDx12 = m_renderContext->As<CRenderContextDx12>();
		rcDx12.SetCommandList(m_commandList);

		//CBR_SVRのディスクリプタのサイズを取得。
		m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		return true;
	}
	void CGraphicsEngineDx12::BeginRender()
	{
		//コマンドアロケータをリセット。
		m_commandAllocator->Reset();
		//コマンドリストもリセット。
		m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get());
		//ビューポートを設定。
		m_commandList->RSSetViewports(1, &m_viewport);
		//シザリング矩形を設定。
		m_commandList->RSSetScissorRects(1, &m_scissorRect);
		//レンダリングターゲットへの書き込み完了待ち
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
		//深度ステンシルバッファのディスクリプタヒープの開始アドレスを取得。
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
		//レンダリングターゲットを設定。
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		m_commandList->ClearDepthStencilView(
			dsvHandle,
			D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
			1.0f,
			0,
			0,
			nullptr);

		
	}
	void CGraphicsEngineDx12::EndRender()
	{
		// Indicate that the back buffer will now be used to present.
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		m_commandList->Close();

		//コマンドを実行。
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		m_swapChain->Present(1, 0);

		//描画完了待ち。
		WaitDraw();
		
	}
	void CGraphicsEngineDx12::Render(
		std::function<void(IRenderContext& rc)> onRender,
		std::function<void(IRenderContext& rc)> onPreForwardRender,
		std::function<void(IRenderContext& rc)> onForwardRender,
		std::function<void(IRenderContext& rc)> onPostRender)
	{
		BeginRender();
		
		onRender( *m_renderContext );
		onPreForwardRender( *m_renderContext );
		onForwardRender( *m_renderContext );
		onPostRender( *m_renderContext );

		EndRender();
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
		D3D_FEATURE_LEVEL fuatureLevel[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0
		};

		for(auto fuatureLevel : fuatureLevel) {

			auto hr = D3D12CreateDevice(
				nullptr,
				fuatureLevel,
				IID_PPV_ARGS(&m_d3dDevice)
			);
			if (SUCCEEDED(hr)) {
				//D3Dデバイスの作成に成功した。
				break;
			}
		}
		if (m_d3dDevice == false) {
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
		swapChainDesc.BufferCount = FRAME_COUNT;
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
	bool CGraphicsEngineDx12::CreateDescriptorHeapForFrameBuffer()
	{
		//RTV用のディスクリプタヒープを作成する。
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
		if (m_rtvHeap == false) {
			//RTV用のディスクリプタヒープの作成に失敗した。
			return false;
		}
		//ディスクリプタのサイズを取得。
		m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//DSV用のディスクリプタヒープを作成する。
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
		if (m_dsvHeap == false) {
			//DSV用のディスクリプタヒープの作成に失敗した。

			return false;
		}
		//ディスクリプタのサイズを取得。
		m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		return true;
	}
	bool CGraphicsEngineDx12::CreateRTVForFameBuffer()
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	
		//フロントバッファをバックバッファ用のRTVを作成。
		for (UINT n = 0; n < FRAME_COUNT; n++) {
			m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_d3dDevice->CreateRenderTargetView(
				m_renderTargets[n].Get(), nullptr, rtvHandle
			);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
		return true;
	}

	bool CGraphicsEngineDx12::CreateDSVForFrameBuffer(const SInitParam& initParam)
	{
		D3D12_CLEAR_VALUE dsvClearValue;
		dsvClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		dsvClearValue.DepthStencil.Depth = 1.0f;
		dsvClearValue.DepthStencil.Stencil = 0;

		CD3DX12_RESOURCE_DESC desc(
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			static_cast<UINT>(initParam.frameBufferWidth),
			static_cast<UINT>(initParam.frameBufferHeight),
			1,
			1,
			DXGI_FORMAT_D32_FLOAT,
			1,
			0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

		auto hr = m_d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&dsvClearValue,
			IID_PPV_ARGS(&m_depthStencilBuffer)
		);
		if (FAILED(hr)) {
			//深度ステンシルバッファの作成に失敗。
			return false;
		}
		//ディスクリプタを作成
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

		m_d3dDevice->CreateDepthStencilView(
			m_depthStencilBuffer.Get(), nullptr, dsvHandle
		);

		return true;
	}
	bool CGraphicsEngineDx12::CreateCommandList()
	{
		//コマンドリストの作成。
		m_d3dDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)
		);
		if (!m_commandList) {
			return false;
		}
		//コマンドリストは開かれている状態で作成されるので、いったん閉じる。
		m_commandList->Close();

		return true;
	}
	bool CGraphicsEngineDx12::CreateSynchronizationWithGPUObject()
	{
		m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		if (!m_fence) {
			//フェンスの作成に失敗した。
			return false;
		}
		m_fenceValue = 1;
		//同期を行うときのイベントハンドラを作成する。
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr) {
			return false;
		}
		return true;
	}
}