#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

namespace tkEngine {
	class CShaderDx12 {
	public:
		/// <summary>
		/// シェーダーをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="entryFuncName">エントリーポイントの関数名。</param>
		/// <param name="shaderModel">シェーダ―モデル。</param>
		void Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel);
		void LoadPS(const wchar_t* filePath, const char* entryFuncName);
		void LoadVS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// コンパイル済みシェーダーデータを取得。
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3DBlob> GetCompiledBlob() const
		{
			return m_blob;
		}
	private:
		ComPtr<ID3DBlob>	m_blob;	//コンパイル済みのシェーダーデータ。
	};

	using UPIShape = std::unique_ptr<IShape>;
}

#endif