#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/Graphics/tkGraphicsInstanceFactory.h"

namespace tkEngine {

	/// <summary>
	/// グラフィック系のプラットフォームに依存するインスタンスのファクトリーのインターフェースクラス。
	/// </summary>
	/// <remarks>
	/// AbstractFactoryパターンで設計されています。
	/// </remarks>
	class CGraphicsInstanceFactoryDx12 : public IGraphicsInstanceFactory {
	public:
		/// <summary>
		/// グラフィックエンジンのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		UPIGraphicsEngine CreateGraphicsEngineInstance() override final;
		/// <summary>
		/// 頂点バッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		UPIVertexBuffer CreateVertexBufferInstance(int size, int stride) override final;
		/// <summary>
		/// インデックスバッファのインスタンスを作成。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		UPIIndexBuffer CreateIndexBufferInstance(int size, int stride) override final;
		/// <summary>
		/// レンダリングコンテキストのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		UPIRenderContext CreateRenderContextInstance()override final;
		/// <summary>
		/// 三角形形状のインスタンスを作成。
		/// </summary>
		/// /// <param name="textureFilePath">
		/// 三角形に貼り付けるテクスチャのファイルパス。
		/// サポートしている画像データはddsファイルのみです。
		/// nullptrを指定した場合は、テクスチャ無しの三角形が表示されます。
		/// </param>
		/// <returns></returns>
		UPIShape CreateTriangleShapeInstance(const wchar_t* textureFilePath)override final;
		/// <summary>
		/// DDSファイルからテクスチャを作成。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <returns>作成されたテクスチャ</returns>
		UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) override final;
	};
}

#endif