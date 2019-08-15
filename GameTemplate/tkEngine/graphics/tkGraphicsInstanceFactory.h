#pragma once


namespace tkEngine {
	
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
		virtual UPIGraphicsEngine CreateGraphicsEngineInstance() = 0;
		/// <summary>
		/// 頂点バッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		virtual UPIVertexBuffer CreateVertexBufferInstance(int size, int stride) = 0;
		/// <summary>
		/// インデックスバッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		virtual UPIIndexBuffer CreateIndexBufferInstance(int size, int stride) = 0;
		/// <summary>
		/// レンダリングコンテキストのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		virtual UPIRenderContext CreateRenderContextInstance() = 0;
		/// <summary>
		/// 三角形形状のインスタンスを作成。
		/// </summary>
		/// <param name="textureFilePath">
		/// 三角形に貼り付けるテクスチャのファイルパス。
		/// サポートしている画像データはddsファイルのみです。
		/// nullptrを指定した場合は、テクスチャ無しの三角形が表示されます。
		/// </param>
		/// <returns></returns>
		virtual UPIShape CreateTriangleShapeInstance(const wchar_t* textureFilePath ) = 0;
		/// <summary>
		/// DDSファイルからテクスチャを作成。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <returns>作成されたテクスチャ</returns>
		virtual UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) = 0;
	};
}