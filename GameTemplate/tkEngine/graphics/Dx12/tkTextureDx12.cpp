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
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();
		//DDSファイルからテクスチャをロード。
		LoadTextureFromDDSFile(filePath, ge12, device);
	
		//テクスチャを利用可能にする。
		SetValid();
	}
	void CTextureDx12::InitFromD3DResource(ComPtr<ID3D12Resource> texture)
	{
		m_texture = texture;
		m_textureDesc = m_texture->GetDesc();
		SetValid();
	}
	void CTextureDx12::InitFromMemory(const char* memory, unsigned int size)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();
		//DDSファイルからテクスチャをロード。
		LoadTextureFromMemory(memory, size, ge12, device);

		//テクスチャを利用可能にする。
		SetValid();
	}
	void CTextureDx12::LoadTextureFromMemory(
		const char* memory,
		unsigned int size,
		CGraphicsEngineDx12& ge12,
		ComPtr<ID3D12Device> device
	)
	{
		DirectX::ResourceUploadBatch re(device.Get());
		re.Begin();
		ID3D12Resource* texture;
		auto hr = DirectX::CreateDDSTextureFromMemoryEx(
			device.Get(),
			re,
			(const uint8_t*)memory,
			size,
			0,
			D3D12_RESOURCE_FLAG_NONE,
			0,
			&texture
		);
		re.End(ge12.GetCommandQueue().Get());

		if (FAILED(hr)) {
			TK_WARNING_MESSAGE_BOX("テクスチャの作成に失敗しました。");
			return;
		}

		m_texture = texture;
		m_textureDesc = m_texture->GetDesc();
	}
	void CTextureDx12::LoadTextureFromDDSFile(
		const wchar_t* filePath,
		CGraphicsEngineDx12& ge12,
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
		re.End(ge12.GetCommandQueue().Get());

		if (FAILED(hr)) {
			TK_WARNING_MESSAGE_BOX("テクスチャの作成に失敗しました。%s", filePath);
			return;
		}

		m_texture = texture;
		m_textureDesc = m_texture->GetDesc();
	}
	
	void CTextureDx12::RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
	{
		if (m_texture) {
			auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
			auto device = ge12.GetD3DDevice();
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = m_textureDesc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = m_textureDesc.MipLevels;
			device->CreateShaderResourceView(m_texture.Get(), &srvDesc, descriptorHandle);
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12