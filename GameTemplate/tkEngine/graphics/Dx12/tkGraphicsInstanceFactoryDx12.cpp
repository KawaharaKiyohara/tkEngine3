#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/Dx12/tkGraphicsInstanceFactoryDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"
#include "tkEngine/graphics/Dx12/tkRenderContextDx12.h"

namespace tkEngine {
	UPIGraphicsEngine CGraphicsInstanceFactoryDx12::CreateGraphicsEngineInstance()
	{
		return std::make_unique< CGraphicsEngineDx12>();
	}
	UPIVertexBuffer CGraphicsInstanceFactoryDx12::CreateVertexBufferInstance(int size, int stride)
	{
		return std::make_unique<CVertexBufferDx12>(size, stride);
	}
	UPIIndexBuffer CGraphicsInstanceFactoryDx12::CreateIndexBufferInstance(int size, int stride)
	{
		return std::make_unique<CIndexBufferDx12>(size, stride);
	}
	UPIRenderContext CGraphicsInstanceFactoryDx12::CreateRenderContextInstance()
	{
		return std::make_unique<CRenderContextDx12>();
	}
}

#endif
