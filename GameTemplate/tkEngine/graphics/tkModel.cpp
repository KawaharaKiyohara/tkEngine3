#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkModel.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkModelDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
#include "tkEngine/graphics/Dx12/tkModelDx11.h"
#endif

namespace tkEngine {
	CModel::CModel()
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		m_imp = std::make_unique<CModelDx12>();
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
		m_imp = std::make_unique<CModelDx11>();
#endif
	}
}