/// <summary>
/// モデルレンダラー
/// </summary>

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/prefab/tkModelRender.h"

namespace tkEngine {
	namespace prefab {
		void CModelRender::Init(const char* filePath)
		{
			m_model.LoadTkmFileAsync(filePath);
			//初期化ステータスをモデル初期化待ちにする。
			m_initStatus = enInitStatus_WaitInitModel;
		}
		bool CModelRender::Start()
		{
			bool inited = false;
			switch (m_initStatus) {
			case enInitStatus_NotCallInitFunc:
				TK_LOG("Please call Init func");
				break;
			case enInitStatus_WaitInitModel:
				if (m_model.IsInited()) {
					//モデルの初期化完了したのでメッシュパーツを構築する。
					m_model.CreateMeshParts();
					m_initStatus = enInitStatus_Completed;
				}
				break;
			case enInitStatus_Completed:
				//初期化完了。
				inited = true;
				break;
			}
			return inited;
		}
		void CModelRender::Update()
		{
			m_model.Update(m_position, m_rotation, m_scale);
		}
		void CModelRender::ForwardRender(IRenderContext& renderContext)
		{
			m_model.Draw(renderContext, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
		}
	}
}