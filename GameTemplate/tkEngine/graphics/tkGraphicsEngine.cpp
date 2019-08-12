#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHIC
S_API_DIRECTX_11
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx11.h"
#endif


namespace tkEngine {
	UPIGraphicsEngine IGraphicsEngine::CreateInstance()
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		return std::make_unique< CGraphicsEngineDx12>();
#endif 
	}
}