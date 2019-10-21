/// <summary>
/// tkEngineのグラフィック関連のヘッダー。
/// </summary>



namespace tkEngine {
	/// <summary>
	/// プリミティブトポロジー。
	/// </summary>
	enum EnPrimitiveTopology {
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		enPrimitiveTopology_TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
#endif
		enPrimitiveTopology_Undex = 0xFFFFFFFF
	};

	/// <summary>
	/// グラフィックス系のユニークポインタ
	/// </summary>
	/// <remarks>
	/// これらは頂点バッファ、インデックスバッファ、テクスチャなどに
	/// アクセスするインターフェースクラスです。
	/// これらのインスタンスはCEngineクラスが保持している、m_graphisInstanceFactoryを
	/// 利用することで作成されます。
	/// </remarks>
	class IShape;
	class IVertexBuffer;
	class IIndexBuffer;
	class IGraphicsEngine;
	class IGraphicsInstanceFactory;
	class ITexture;
	class IMeshParts;
	class IRenderContext;
	class ILightManager;

	using UPIGraphicsEngine = std::unique_ptr< IGraphicsEngine>;
	using UPIVertexBuffer = std::unique_ptr < IVertexBuffer >;
	using UPIIndexBuffer = std::unique_ptr<IIndexBuffer>;
	using UPIRenderContext = std::unique_ptr<IRenderContext>;
	using UPIGraphicsInstanceFactory = std::unique_ptr< IGraphicsInstanceFactory>;
	using UPIShape = std::unique_ptr<IShape>;
	using UPITexture = std::unique_ptr<ITexture>;
	using UPIMeshParts = std::unique_ptr<IMeshParts>;
	using UPILightManager = std::unique_ptr< ILightManager >;
}

