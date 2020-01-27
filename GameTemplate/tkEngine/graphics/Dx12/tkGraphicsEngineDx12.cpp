#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkEngine/gameObject/tkGameObjectManager.h"

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
	bool CGraphicsEngineDx12::OnInit(HWND hwnd, const SInitParam& initParam)
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
			TK_ASSERT(false, "D3D�f�o�C�X�̍쐬�Ɏ��s���܂����B");
			return false;
		}

		//�R�}���h�L���[�̍쐬�B
		if (!CreateCommandQueue()) {
			//�R�}���h�L���[�̍쐬�Ɏ��s�����B
			TK_ASSERT(false, "�R�}���h�L���[�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�X���b�v�`�F�C���̍쐬�B
		if (!CreateSwapChain(hwnd, initParam, dxGiFactory)) {
			//�X���b�v�`�F�C���̍쐬�Ɏ��s�����B
			TK_ASSERT(false, "�X���b�v�`�F�C���̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�t���[���o�b�t�@��RTV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
		if (!CreateDescriptorHeapForFrameBuffer()) {
			TK_ASSERT(false, "�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�t���[���o�b�t�@�p��RTV���쐬����B
		if (!CreateRTVForFameBuffer()) {
			TK_ASSERT(false, "�t���[���o�b�t�@�p��RTV�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�t���[���o�b�t�@�p��DSV���쐬����B
		if (!CreateDSVForFrameBuffer(initParam)) {
			TK_ASSERT(false, "�t���[���o�b�t�@�p��DSV�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�R�}���h�A���P�[�^�̍쐬�B
		m_d3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, 
			IID_PPV_ARGS(&m_commandAllocator));
		if (!m_commandAllocator) {
			TK_ASSERT(false, "�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		
		//�R�}���h���X�g�̍쐬�B
		if (!CreateCommandList()) {
			TK_ASSERT(false, "�R�}���h���X�g�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//GPU�Ɠ������Ƃ邽�߂̃I�u�W�F�N�g���쐬����B
		if (!CreateSynchronizationWithGPUObject()) {
			TK_ASSERT(false, "GPU�Ƃ̓����I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//���C�������_�����O�^�[�Q�b�g���쐬�B
		float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		if (m_mainRenderTarget.Create(
			initParam.frameBufferWidth,
			initParam.frameBufferHeight,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor) == false) {
			TK_ASSERT(false, "���C�������_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�p�C�v���C���X�e�[�g���������B
		CPipelineStatesDx12::Init();

		//�|�X�g�G�t�F�N�g�̏������B
		m_postEffect.Init(initParam.graphicsConfing);

		m_copyFullScreenSprite.Init(&m_mainRenderTarget.GetRenderTargetTexture(), initParam.frameBufferWidth, initParam.frameBufferHeight);
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
		auto giFactry = g_engine->GetGraphicsInstanceFactory();
		m_renderContext = giFactry->CreateRenderContext();
		auto& rcDx12 = m_renderContext->As<CRenderContextDx12>();
		rcDx12.SetCommandList(m_commandList);

		//CBR_SVR�̃f�B�X�N���v�^�̃T�C�Y���擾�B
		m_cbrSrvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//�f�B�X�N���v�^�q�[�v�v�[���̏������B
		m_descriptorHeapPool.Init();

		return true;
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
			//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			return false;
		}
		//�f�B�X�N���v�^�̃T�C�Y���擾�B
		m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//DSV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		m_d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
		if (m_dsvHeap == false) {
			//DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B

			return false;
		}
		//�f�B�X�N���v�^�̃T�C�Y���擾�B
		m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
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
			//�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s�B
			return false;
		}
		//�f�B�X�N���v�^���쐬
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();

		m_d3dDevice->CreateDepthStencilView(
			m_depthStencilBuffer.Get(), nullptr, dsvHandle
		);

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
	void CGraphicsEngineDx12::BeginRender()
	{
		auto& rcDx12 = m_renderContext->As<CRenderContextDx12>();
		//�f�B�X�N���v�^�q�[�v�v�[���̃��Z�b�g�B
		m_descriptorHeapPool.Reset();
		//�R�}���h�A���P�[�^�����Z�b�g�B
		m_commandAllocator->Reset();
		//�����_�����O�q�e�L�X�g�����Z�b�g�B
		rcDx12.Reset(m_commandAllocator, m_pipelineState);
		//�r���[�|�[�g��ݒ�B
		rcDx12.SetViewport(m_viewport);
		//�V�U�����O��`��ݒ�B
		rcDx12.SetScissorRect(m_scissorRect);

		//�����_�����O�^�[�Q�b�g�����C���ɂ���B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�\�ɂȂ�܂ő҂B
		rcDx12.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		rcDx12.SetRenderTarget(m_mainRenderTarget);
		const float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		rcDx12.ClearRenderTargetView(m_mainRenderTarget, clearColor);
		rcDx12.ClearDepthStencilView(m_mainRenderTarget, 1.0f);
	}
	void CGraphicsEngineDx12::EndRender()
	{

		auto& rcDx12 = m_renderContext->As<CRenderContextDx12>();

		//�o�b�N�o�b�t�@�Ƀe�N�X�`�����R�s�[�B
		{
			//���C�������_�����O�^�[�Q�b�g�ւ̏������ݑ҂��B
			rcDx12.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
			//�o�b�N�o�b�t�@�������_�����O�^�[�Q�b�g�Ƃ��Đݒ�\�ɂȂ�܂ő҂B
			rcDx12.WaitUntilToPossibleSetRenderTarget(m_renderTargets[m_frameIndex]);

			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
			rtvHandle.ptr += m_frameIndex * m_rtvDescriptorSize;
			//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�A�h���X���擾�B
			D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
			//�����_�����O�^�[�Q�b�g��ݒ�B
			rcDx12.SetRenderTarget(rtvHandle, dsvHandle);

			const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
			rcDx12.ClearRenderTargetView(rtvHandle, clearColor);
			rcDx12.ClearDepthStencilView(dsvHandle, 1.0f);

			//�R�s�[�p�̃p�C�v���C���X�e�[�g��ݒ肷��B
			rcDx12.SetRootSignature(CPipelineStatesDx12::m_modelDrawRootSignature);
			rcDx12.SetPipelineState(CPipelineStatesDx12::m_copyMainTargetToFrameBufferPipeline);

			m_copyFullScreenSprite.Draw(rcDx12, m_cameraPostEffect.GetViewMatrix(), m_cameraPostEffect.GetProjectionMatrix());
			// �����_�����O�^�[�Q�b�g�ւ̕`�����݊����҂�
			rcDx12.WaitUntilFinishDrawingToRenderTarget(m_renderTargets[m_frameIndex]);
		}

		//�����_�����O�R���e�L�X�g�����B
		rcDx12.Close();

		//�R�}���h�����s�B
		ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		// Present the frame.
		m_swapChain->Present(1, 0);

		//�`�抮���҂��B
		WaitDraw();

	}
	void CGraphicsEngineDx12::OnRender(CGameObjectManager* goMgr)
	{
		BeginRender();

		auto& rcDx12 = m_renderContext->As<CRenderContextDx12>();
		//�V���h�E�}�b�v�ւ̃����_�����O�B
		goMgr->RenderToShadowMap(rcDx12);

		//G-Buffer�ւ̃����_�����O�p�X�B
		goMgr->RenderGBuffer(rcDx12);

		//�t�H���[�h�����_�����O�p�X�B
		rcDx12.SetRootSignature(CPipelineStatesDx12::m_modelDrawRootSignature);
		goMgr->ForwardRender(rcDx12);

		//�|�X�g�G�t�F�N�g
		m_postEffect.Render(rcDx12);

		//HUD�`��p�X�B
		//HUD�`��p�̃p�C�v���C���X�e�[�g��ݒ肷��B
		rcDx12.SetRootSignature(CPipelineStatesDx12::m_modelDrawRootSignature);
		rcDx12.SetPipelineState(CPipelineStatesDx12::m_spritePipeline);
		goMgr->RenderHUD(rcDx12);

		EndRender();
	}
}