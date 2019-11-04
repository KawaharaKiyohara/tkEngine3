#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/graphics/tkLightManager.h"

namespace tkEngine {

	bool IGraphicsEngine::Init(HWND hwnd, const SInitParam& initParam)
	{
		m_frameBufferWidth = initParam.frameBufferWidth;
		m_frameBufferHeight = initParam.frameBufferHeight;
		if (!OnInit(hwnd, initParam) ) {
			//初期化失敗。
			return false;
		}
		//ライトマネージャを作成。
		m_lightManager = Engine().GetGraphicsInstanceFactory()->CreateLightManager();
		m_lightManager->Init();
		//2Dカメラの初期化。
		m_camera2D.SetTarget(CVector3::Zero);
		m_camera2D.SetPosition({ 0.0f, 0.0f, -10.0f });
		m_camera2D.SetUpdateProjMatrixFunc(CCamera::enUpdateProjMatrixFunc_Ortho);
		m_camera2D.SetWidth(m_2dSpaceScreenWidth);
		m_camera2D.SetHeight(m_2dSpaceScreenHeight);
		m_camera2D.SetNear(0.1f);
		m_camera2D.SetFar(1000.0f);
		m_camera2D.Update();

		m_cameraPostEffect.SetTarget(CVector3::Zero);
		m_cameraPostEffect.SetPosition({ 0.0f, 0.0f, -10.0f });
		m_cameraPostEffect.SetUpdateProjMatrixFunc(CCamera::enUpdateProjMatrixFunc_Ortho);
		m_cameraPostEffect.SetWidth(m_frameBufferWidth);
		m_cameraPostEffect.SetHeight(m_frameBufferHeight);
		m_cameraPostEffect.SetNear(0.1f);
		m_cameraPostEffect.SetFar(1000.0f);
		m_cameraPostEffect.Update();
		//3Dカメラの初期化。
		m_camera3D.SetTarget({ 0.0f, 50.0f, 0.0f });
		m_camera3D.SetNear(0.1f);
		m_camera3D.SetFar(10000.0f);
		m_camera3D.SetPosition({ 0.0f, 50.0f, 200.0f });
		m_camera3D.SetUp(g_vec3AxisY);
		return true;
	}
	void IGraphicsEngine::Update()
	{
		//ライトの更新。
		m_lightManager->Update();
		//カメラの更新。
		m_camera3D.Update();
		//2Dカメラの更新。
		m_camera2D.Update();
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