#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
	#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx11.h"
#endif

namespace tkEngine {
	std::unique_ptr<IIndexBuffer> IIndexBuffer::Create(int size, int stride)
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		return std::make_unique<CIndexBuffer>(size, stride);
#else

#endif
	}
}