/// <summary>
/// tkEngineのグラフィック関連のヘッダー。
/// </summary>

/// <summary>
/// プリミティブトポロジー。
/// </summary>
enum EnPrimitiveTopology {
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	enPrimitiveTopology_TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
#endif
	enPrimitiveTopology_Undex = 0xFFFFFFFF
};

