#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


#include "tkEngine/graphics/tkMeshParts.h"
namespace tkEngine {
	class CMeshPartsDx12 : public IMeshParts {
	public:
		explicit CMeshPartsDx12(const CTkmFile& tkmFile) :
			IMeshParts(tkmFile) {}
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
