#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/graphics/tkLightManager.h"

namespace tkEngine {

	bool IGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
		m_frameBufferWidth = initParam.frameBufferWidth;
		m_frameBufferHeight = initParam.frameBufferHeight;
		if (OnInit(hwnd, initParam) ) {
			//ライトマネージャを作成。
			m_lightManager = Engine().GetGraphicsInstanceFactory()->CreateLightManager();
			m_lightManager->Init();
			return true;
		}
		return false;
	}
	void IGraphicsEngine::Update()
	{
		//ライトの更新。
		m_lightManager->Update();
	}
	void IGraphicsEngine::Render(
		std::function<void(IRenderContext & rc)> onRender,
		std::function<void(IRenderContext & rc)> onPreForwardRender,
		std::function<void(IRenderContext & rc)> onForwardRender,
		std::function<void(IRenderContext & rc)> onPostRender
	)
	{
		m_lightManager->Render(*m_renderContext);
		OnRender( onRender, onPreForwardRender, onForwardRender, onPostRender);
	}
}