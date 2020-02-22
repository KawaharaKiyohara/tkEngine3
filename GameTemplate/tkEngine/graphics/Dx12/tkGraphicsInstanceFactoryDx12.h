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
		/// グラフィックエンジンを作成。
		/// </summary>
		/// <returns></returns>
		UPIGraphicsEngine CreateGraphicsEngine() override final;
		/// <summary>
		/// 頂点バッファの作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		UPIVertexBuffer CreateVertexBuffer(int size, int stride) override final;
		/// <summary>
		/// インデックスバッファを作成。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド</param>
		/// <returns></returns>
		UPIIndexBuffer CreateIndexBuffer(int size, int stride) override final;
		/// <summary>
		/// レンダリングコンテキストを作成。
		/// </summary>
		/// <returns></returns>
		UPIRenderContext CreateRenderContext()override final;
		/// <summary>
		/// DDSファイルからテクスチャを作成。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <returns>作成されたテクスチャ</returns>
		UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) override final;
		/// <summary>
		/// TKMファイルからメッシュパーツを作成。
		/// </summary>
		/// <param name="tkmFile">tkmファイル</param>
		/// <returns>作成されたメッシュパーツ。</returns>
		UPIMeshParts CreateMeshPartsFromTkmFile(const CTkmFile& tkmFile) override final;
		/// <summary>
		/// ライトマネージャーを作成。
		/// </summary>
		/// <returns>作成されたライトマネージャ。</returns>
		UPILightManager CreateLightManager() override final;
		/// <summary>
		/// 指向性シャドウマップのインスタンスを作成。
		/// </summary>
		/// <returns></returns>
		UPIDirectionalShadowMap CreateDirectionalShadowMap() override final;

	};
}

#endif