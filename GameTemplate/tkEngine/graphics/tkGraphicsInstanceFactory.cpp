#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsInstanceFactory.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkGraphicsInstanceFactoryDx12.h"
#endif

namespace tkEngine {
	/// <summary>
	/// �t�@�N�g���̃C���X�^���X���쐬�B
	/// </summary>
	/// <returns></returns>
	UPIGraphicsInstanceFactory IGraphicsInstanceFactory::CreateInstance()
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		return std::make_unique<CGraphicsInstanceFactoryDx12>();
#endif
	}
}