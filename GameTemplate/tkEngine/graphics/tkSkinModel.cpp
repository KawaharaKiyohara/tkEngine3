#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkSkinModel.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkSkinModelDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
#include "tkEngine/graphics/Dx12/tkSkinModelDx11.h"
#endif

namespace tkEngine {
	CSkinModel::CSkinModel()
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		m_imp = std::make_unique<CSkinModelDx12>();
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
		m_imp = std::make_unique<CSkinModelDx11>();
#endif
	}
}