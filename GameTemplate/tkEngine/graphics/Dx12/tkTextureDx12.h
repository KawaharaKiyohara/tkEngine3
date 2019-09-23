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
		/// メモリからテクスチャを初期化する。
		/// </summary>
		/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス</param>
		/// <param name="size">テクスチャのサイズ。</param>
		void InitFromMemory(const char* memory, unsigned int size);
		
		/// <summary>
		/// SRVに登録。
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
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
		/// メモリからテクスチャをロード。
		/// </summary>
		/// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス。</param>
		/// <param name="size">テクスチャのサイズ。</param>
		/// <param name="ge12">Dx12版のグラフィックスエンジン</param>
		/// <param name="device">D3Dデバイス</param>
		void LoadTextureFromMemory(
			const char* memory, 
			unsigned int size,
			CGraphicsEngineDx12* ge12,
			ComPtr<ID3D12Device> device
		);
		
	private:
		ComPtr< ID3D12Resource>	m_texture;	//テクスチャ。
		D3D12_RESOURCE_DESC m_textureDesc;	//テクスチャ情報
	};
}