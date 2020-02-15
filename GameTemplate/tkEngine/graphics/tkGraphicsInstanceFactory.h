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
		/// 三角形形状のインスタンスを作成。
		/// </summary>
		/// <param name="textureFilePath">
		/// 三角形に貼り付けるテクスチャのファイルパス。
		/// サポートしている画像データはddsファイルのみです。
		/// nullptrを指定した場合は、テクスチャ無しの三角形が表示されます。
		/// </param>
		/// <returns></returns>
		virtual UPIShape CreateTriangleShape(const wchar_t* textureFilePath ) = 0;
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