#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"

namespace tkEngine {
	CTextureDx12::CTextureDx12(const wchar_t* filePath)
	{
		InitFromDDSFile(filePath);
	}
	void CTextureDx12::InitFromDDSFile(const wchar_t* filePath)
	{
		auto ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12->GetD3DDevice();
		//DDSファイルからテクスチャをロード。
		LoadTextureFromDDSFile(filePath, ge12, device);
		//ディスクリプタヒープの初期化。
		InitSrvDescriptorHeap(device);
		//テクスチャを利用可能にする。
		SetValid();
	}
	void CTextureDx12::LoadTextureFromDDSFile(
		const wchar_t* filePath,
		CGraphicsEngineDx12* ge12,
		ComPtr<ID3D12Device> device
	)
	{
		DirectX::ResourceUploadBatch re(device.Get());
		re.Begin();
		ID3D12Resource* texture;
		auto hr = DirectX::CreateDDSTextureFromFileEx(
			device.Get(),
			re,
			filePath,
			0,
			D3D12_RESOURCE_FLAG_NONE,
			0,
			&texture
		);
		re.End(ge12->GetCommandQueue().Get());

		if (FAILED(hr)) {
			TK_WARNING_MESSAGE_BOX("テクスチャの作成に失敗しました。%s", filePath);
			return;
		}

		m_texture = texture;
		m_textureDesc = m_texture->GetDesc();
	}
	void CTextureDx12::InitSrvDescriptorHeap(ComPtr<ID3D12Device> device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		auto hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
		
		TK_ASSERT(SUCCEEDED(hr), "CTextureDx12::InitSrvDescriptorHeap：ディスクリプタヒープの作成に失敗しました。");

		
		//ディスクリプタヒープとテクスチャを関連付ける。
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = m_textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_textureDesc.MipLevels;
		device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12