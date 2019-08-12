#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
	#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx11.h"
#endif

namespace tkEngine {
	UPIIndexBuffer IIndexBuffer::Create(int size, int stride)
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		auto ib = std::make_unique<CIndexBufferDx12>(size, stride);
#else
#endif
		ib->m_count = size / stride;
		return ib;
	}
}