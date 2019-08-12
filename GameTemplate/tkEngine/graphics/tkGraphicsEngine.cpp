#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"
#elif TK_GRAPHICS_API == TK_GRAPHIC
S_API_DIRECTX_11
	#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx11.h"
#endif


namespace tkEngine {
	bool CGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		m_imp = std::make_unique <CGraphicsEngineDx12>();
#elif TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_11
#endif
		return m_imp->Init(hwnd, initParam);
	}
	void CGraphicsEngine::Render(
		std::function<void()> onRender,
		std::function<void()> onPreForwardRender,
		std::function<void()> onForwardRender,
		std::function<void()> onPostRender
	)
	{
		m_imp->Render( onRender, onPreForwardRender, onForwardRender, onPostRender );
	}
	void CGraphicsEngine::Destroy()
	{
		m_imp->Destroy();
	}
}