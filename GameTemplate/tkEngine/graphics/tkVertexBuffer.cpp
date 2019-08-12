#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
	#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx11.h"
#endif

namespace tkEngine {
	UPIVertexBuffer IVertexBuffer::Create(int size, int stride)
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		return std::make_unique<CVertexBufferDx12>(size, stride);
#else

#endif
	}
}