#pragma once


namespace tkEngine {
	class CTkmFile;
	/// <summary>
	/// グラフィック系のプラットフォームに依存するインスタンスのファクトリーのインターフェースクラス。
	/// </summary>
	/// <remarks>
	/// AbstractFactoryパターンで設計されています。
	/// </remarks>
	class IGraphicsInstanceFactory {
	public:
		/// <summary>
		/// ファクトリのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		static UPIGraphicsInstanceFactory CreateInstance();
		/// <summary>
		/// グラフィックエンジンのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		virtual UPIGraphicsEngine CreateGraphicsEngine() = 0;
		/// <summary>
		/// 頂点バッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		virtual UPIVertexBuffer CreateVertexBuffer(int size, int stride) = 0;
		/// <summary>
		/// インデックスバッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		virtual UPIIndexBuffer CreateIndexBuffer(int size, int stride) = 0;
		/// <summary>
		/// レンダリングコンテキストのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		virtual UPIRenderContext CreateRenderContext() = 0;
		/// <summary>
		/// DDSファイルからテクスチャを作成。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <returns>作成されたテクスチャ</returns>
		virtual UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) = 0;
		/// <summary>
		/// TKMファイルからメッシュパーツを作成。
		/// </summary>
		/// <param name="tkmFile">tkmファイル</param>
		/// <returns>作成されたメッシュパーツ。</returns>
		virtual UPIMeshParts CreateMeshPartsFromTkmFile(const CTkmFile& tkmFile) = 0;
		/// <summary>
		/// ライトマネージャーを作成。
		/// </summary>
		/// <returns>作成されたライトマネージャ。</returns>
		virtual UPILightManager CreateLightManager() = 0;
		/// <summary>
		/// 指向性シャドウマップのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		virtual UPIDirectionalShadowMap CreateDirectionalShadowMap() = 0;
	};
}