#pragma once

namespace tkEngine {
	class CGraphicsEngineDx12;
	class CTextureDx12 : public ITexture{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CTextureDx12() {}
		/// <summary>
		/// ファイルからテクスチャをロードするコンストラクタ
		/// </summary>
		/// <param name="filePath">ロードするテクスチャのファイルパス。</param>
		explicit CTextureDx12(const wchar_t* filePath);
		/// <summary>
		/// DDSファイルからテクスチャを初期化する。
		/// </summary>
		/// <param name="filePath">ロードするテクスチャのファイルパス。</param>
		void InitFromDDSFile(const wchar_t* filePath);
		/// <summary>
		/// SRVのディスクリプタヒープを取得する。
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12DescriptorHeap> GetDiscriptorHeap() const
		{
			return m_srvHeap;
		}
	private:
		/// <summary>
		/// DDSファイルからテクスチャをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="ge12">Dx12版のグラフィックスエンジン</param>
		/// <param name="device">D3Dデバイス</param>
		void LoadTextureFromDDSFile(
			const wchar_t* filePath, 
			CGraphicsEngineDx12* ge12, 
			ComPtr<ID3D12Device> device
		);
		/// <summary>
		/// SRV用のディスクリプタヒープの初期化。
		/// </summary>
		/// <param name="device">D3Dデバイス</param>
		void InitSrvDescriptorHeap(ComPtr<ID3D12Device> device);
	private:
		ComPtr< ID3D12DescriptorHeap> m_srvHeap;	//SRVヒープ。
		ComPtr< ID3D12Resource>	m_texture;	//テクスチャ。
		D3D12_RESOURCE_DESC m_textureDesc;	//テクスチャ情報
	};
}