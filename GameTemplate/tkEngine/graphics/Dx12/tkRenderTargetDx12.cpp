#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkRenderTargetDx12.h"

namespace tkEngine {
	bool CRenderTargetDx12::Create(
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT colorFormat,
		DXGI_FORMAT depthStencilFormat
	)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		m_width = w;
		m_height = h;
		//�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬����B
		if (!CreateRenderTargetTexture(ge12, d3dDevice, w, h, mipLevel, arraySize, colorFormat)) {
			TK_ASSERT(false, "�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`���̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�����쐬����B
		if (depthStencilFormat != DXGI_FORMAT_UNKNOWN) {
			if (!CreateDepthStencilTexture(ge12, d3dDevice, w, h, depthStencilFormat)) {
				TK_ASSERT(false, "�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`���̍쐬�Ɏ��s���܂����B");
				return false;
			}
		}
		if (!CreateDescriptorHeap(ge12, d3dDevice)) {
			//�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			TK_ASSERT(false, "�f�B�X�N�v���^�q�[�v�̍쐬�Ɏ��s���܂����B");
			return false;
		}
		//�f�B�X�N���v�^���쐬����B
		CreateDescriptor(d3dDevice);

		return true;
	}
	bool CRenderTargetDx12::CreateDescriptorHeap(CGraphicsEngineDx12& ge, ComPtr<ID3D12Device>& d3dDevice)
	{
		
		//RTV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = CGraphicsEngineDx12::FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap));
		if (m_rtvHeap == false) {
			//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
			return false;
		}
		//�f�B�X�N���v�^�̃T�C�Y���擾�B
		m_rtvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		if (m_depthStencilTexture) {
			//DSV�p�̃f�B�X�N���v�^�q�[�v���쐬����B
			desc.NumDescriptors = 1;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			d3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap));
			if (m_dsvHeap == false) {
				//DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
				return false;
			}
			//�f�B�X�N���v�^�̃T�C�Y���擾�B
			m_dsvDescriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}
		return true;
	}
	bool CRenderTargetDx12::CreateRenderTargetTexture(
		CGraphicsEngineDx12& ge,
		ComPtr<ID3D12Device>& d3dDevice,
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT format
	)
	{
		CD3DX12_RESOURCE_DESC desc(
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			static_cast<UINT>(w),
			static_cast<UINT>(h),
			arraySize,
			mipLevel,
			format,
			1,
			0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		D3D12_CLEAR_VALUE clearValue;
		clearValue.Format = format;
		clearValue.Color[0] = 0.0f; 
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;

		//���\�[�X���쐬�B
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COMMON,
			&clearValue,
			IID_PPV_ARGS(&m_renderTargetTextureDx12)
		);

		if (FAILED(hr)) {
			//�쐬�Ɏ��s�B
			return false;
		}
		m_renderTargetTexture.InitFromD3DResource(m_renderTargetTextureDx12);
		return true;
	}
	bool CRenderTargetDx12::CreateDepthStencilTexture(
		CGraphicsEngineDx12& ge,
		ComPtr<ID3D12Device>& d3dDevice,
		int w,
		int h,
		DXGI_FORMAT format)
	{
		D3D12_CLEAR_VALUE dsvClearValue;
		dsvClearValue.Format = format;
		dsvClearValue.DepthStencil.Depth = 1.0f;
		dsvClearValue.DepthStencil.Stencil = 0;

		CD3DX12_RESOURCE_DESC desc(
			D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			0,
			static_cast<UINT>(w),
			static_cast<UINT>(h),
			1,
			1,
			format,
			1,
			0,
			D3D12_TEXTURE_LAYOUT_UNKNOWN,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&dsvClearValue,
			IID_PPV_ARGS(&m_depthStencilTexture)
		);
		if (FAILED(hr)) {
			//�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s�B
			return false;
		}
		return true;
	}
	void CRenderTargetDx12::CreateDescriptor(ComPtr<ID3D12Device>& d3dDevice)
	{
		//�J���[�e�N�X�`���̃f�B�X�N���v�^���쐬�B
		auto rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		d3dDevice->CreateRenderTargetView(m_renderTargetTexture.Get(), nullptr, rtvHandle);
		if (m_depthStencilTexture) {
			//�[�x�e�N�X�`���̃f�B�X�N���v�^���쐬
			auto dsvHandle = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
			d3dDevice->CreateDepthStencilView(m_depthStencilTexture.Get(), nullptr, dsvHandle);
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
