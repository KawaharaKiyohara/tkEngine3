#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkIPrimitive.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/Dx12/tkPrimitiveDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
#include "tkEngine/graphics/Dx11/tkPrimitiveDx11.h"
#endif


namespace tkEngine {
	std::unique_ptr<IPrimitive> IPrimitive::Create(
		int vertexBufferSize,
		int vertexBufferStride,
		int indexBufferSize,
		int indexBufferStride,
		EnPrimitiveTopology enPrimTopology)
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		auto primitive = std::make_unique<CPrimitiveDx12>();
#endif
		//頂点バッファとインデックスバッファを作成。
		primitive->m_vertexBuffer = IVertexBuffer::Create(vertexBufferSize, vertexBufferStride);
		primitive->m_indexBuffer = IIndexBuffer::Create(indexBufferSize, indexBufferStride);
		primitive->m_topology = enPrimTopology;
		return primitive;
	}
}