#pragma once

namespace tkEngine {
	class CRenderContextBase : Noncopyable {

	};

}
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/dx12/tkRenderContextDx12.h"
#else
#endif