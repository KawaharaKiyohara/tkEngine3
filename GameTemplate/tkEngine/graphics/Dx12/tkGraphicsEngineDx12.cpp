#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"

namespace tkEngine {
	
	void CGraphicsEngineDx12::WaitDraw()
	{
		//�`��I���҂�
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
	bool CGraphicsEngineDx12::Init(HWND hwnd, const SInitParam& initParam)
	{
		//DXGI�t�@�N�g���[�̍쐬�B
		auto dxGiFactory = CreateDXGIFactory();
		if (!dxGiFactory ) {
			//�t�@�N�g���̍쐬�Ɏ��s�����B
			return false;
		}
		//D3D�f�o�C�X�̍쐬�B
		if (!CreateD3DDevice()) {
			//�f�o�C�X�̍쐬�Ɏ��s�����B
			TK_WARNING_MESSAGE_BOX("D3D�f�o�C�X�̍쐬�Ɏ��s���܂����B");
			return false;
		}

		//�R�}���h�L���[�̍쐬�B
		if (!CreateCommandQueue()) {
			//�R�}���h�L���[�̍쐬�Ɏ��s�����B
			TK_WARNING_MESSAGE_BOX("�R�}���h�L���[�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�X���b�v�`�F�C���̍쐬�B
		if (!CreateSwapChain(hwnd, initParam, dxGiFactory)) {
			//�X���b�v�`�F�C���̍쐬�Ɏ��s�����B
			TK_WARNING_MESSAGE_BOX("�X���b�v�`�F�C���̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�t���[���o�b�t�@��RTV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
		if (!CreateDescriptorHeapForFrameBuffer()) {
			TK_WARNING_MESSAGE_BOX("�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�t���[���o�b�t�@�p��RTV���쐬����B
		if (!CreateRTVForFameBuffer()) {
			TK_WARNING_MESSAGE_BOX("�t���[���o�b�t�@�p��RTV�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�R�}���h�A���P�[�^�̍쐬�B
		m_d3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(&m_commandAllocator));
		if (!m_commandAllocator) {
			TK_WARNING_MESSAGE_BOX("�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		
		//�R�}���h���X�g�̍쐬�B
		if (!CreateCommandList()) {
			TK_WARNING_MESSAGE_BOX("�R�}���h���X�g�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//GPU�Ɠ������Ƃ邽�߂̃I�u�W�F�N�g���쐬����B
		if (!CreateSynchronizationWithGPUObject()) {
			TK_WARNING_MESSAGE_BOX("GPU�Ƃ̓����I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�r���[�|�[�g���������B
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.Width = static_cast<float>(initParam.frameBufferWidth);
		m_viewport.Height = static_cast<float>(initParam.frameBufferHeight);
		m_viewport.MinDepth = D3D12_MIN_DEPTH;
		m_viewport.MaxDepth = D3D12_MAX_DEPTH;

		//�V�U�����O��`���������B
		m_scissorRect.left = 0;
		m_scissorRect.top = 0;
		m_scissorRect.right = initParam.frameBufferWidth;
		m_scissorRect.bottom = initParam.frameBufferHeight;

		//�����_�����O�R���e�L�X�g�̍쐬�B
		auto giFactry = Engine().GetGraphicsInstanceFactory();
		m_renderContext = giFactry->CreateRenderContextInstance();
		auto rcDx12 = m_renderContext->As<CRenderContextDx12>();
		rcDx12->SetCommandList(m_commandList);

		return true;
	}
	void CGraphicsEngineDx12::BeginRender()
	{
		//�R�}���h�A���P�[�^�����Z�b�g�B
		m_commandAllocator->Reset();
		//�R�}���h���X�g�����Z�b�g�B
		m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get());
		//�r���[�|�[�g��ݒ�B
		m_commandList->RSSetViewports(1, &m_viewport);
		//�V�U�����O��`��ݒ�B
		m_commandList->RSSetScissorRects(1, &m_scissorRect);
		//�����_�����O�^�[�Q�b�g�ւ̏������݊����҂�
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
		//�����_�����O�^�[�Q�b�g��ݒ�B
		m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		
	}
	void CGraphicsEngineDx12::EndRender()
	{
		// Indicate that the back buffer will now be used to present.
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		m_commandList->Close();

		//�R�}���h�����s�B
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		m_swapChain->Present(1, 0);

		//�`�抮���҂��B
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
		//�f�o�b�O�R���g���[���[������΁A�f�o�b�O���C���[������DXGI���쐬����B
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
				//D3D�f�o�C�X�̍쐬�ɐ��������B
				break;
			}
		}
		if (m_d3dDevice == false) {
			//D3D�f�o�C�X�̍쐬�Ɏ��s�����B
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
			//�R�}���h�L���[�̍쐬�Ɏ��s�����B
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
		//IDXGISwapChain3�̃C���^�[�t�F�[�X���擾�B
		swapChain.As(&m_swapChain);
		m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
		return true;
	}
	bool CGraphicsEngineDx12::CreateDescriptorHeapForFrameBuffer()
	{
		//RTV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
		if (m_rtvHeap == false) {
			//�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			return false;
		}
		//�f�B�X�N���v�^�q�[�v�̃T�C�Y���擾�B
		m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		return true;
	}
	bool CGraphicsEngineDx12::CreateRTVForFameBuffer()
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	
		//�t�����g�o�b�t�@���o�b�N�o�b�t�@�p��RTV���쐬�B
		for (UINT n = 0; n < FRAME_COUNT; n++) {
			m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n]));
			m_d3dDevice->CreateRenderTargetView(
				m_renderTargets[n].Get(), nullptr, rtvHandle
			);
			rtvHandle.ptr += m_rtvDescriptorSize;
		}
		return true;
	}
	bool CGraphicsEngineDx12::CreateCommandList()
	{
		//�R�}���h���X�g�̍쐬�B
		m_d3dDevice->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)
		);
		if (!m_commandList) {
			return false;
		}
		//�R�}���h���X�g�͊J����Ă����Ԃō쐬�����̂ŁA�����������B
		m_commandList->Close();

		return true;
	}
	bool CGraphicsEngineDx12::CreateSynchronizationWithGPUObject()
	{
		m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		if (!m_fence) {
			//�t�F���X�̍쐬�Ɏ��s�����B
			return false;
		}
		m_fenceValue = 1;
		//�������s���Ƃ��̃C�x���g�n���h�����쐬����B
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr) {
			return false;
		}
		return true;
	}
}