#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"

namespace tkEngine {

	bool IGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
		m_frameBufferWidth = initParam.frameBufferWidth;
		m_frameBufferHeight = initParam.frameBufferHeight;
		return OnInit(hwnd, initParam);
	}
}