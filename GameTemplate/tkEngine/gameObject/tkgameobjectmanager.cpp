/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/gameObject/tkgameobjectmanager.h"
#include "tkEngine/graphics/tkRenderContext.h"
#include <future>

namespace tkEngine{
	void CGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}
	void CGameObjectManager::PreUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}
	void CGameObjectManager::Update()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->UpdateWrapper();
			}
		}
	}
	void CGameObjectManager::PostUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}
	void CGameObjectManager::BeginRender(IRenderContext& rc)
	{
		TK_WARNING("CGameObjectManager::Begin Render���������B");
	}

	void CGameObjectManager::ForwardRender(IRenderContext& rc)
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->ForwardRenderWrapper(rc);
			}
		}
	}
	void CGameObjectManager::RenderToShadowMap(IRenderContext& rc)
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				//todo �J�����s��ƃv���W�F�N�V�����s��͓K���B
				obj->RenderToShadowMapWrapper(rc, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
			}
		}
	}
	void CGameObjectManager::RenderHUD(IRenderContext& rc)
	{
	
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->RenderHUD(rc);
			}
		}
	}
	void CGameObjectManager::ExecuteFromGameThread()
	{
		
		//�X�V�n�̏����B
		{
			//Start
			Start();
			//���O�A�b�v�f�[�g�B
			PreUpdate();
			//�A�b�v�f�[�g�B
			Update();
			//�x���A�b�v�f�[�g�B
			PostUpdate();
		}
		//�`��n�̏����B
		{
			g_graphicsEngine->Render(this);
		}
		ExecuteDeleteGameObjects();
	}

	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//�o�b�t�@��؂�ւ��B
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for(GameObjectList& goList : m_deleteObjectArray[preBufferNo]){
			for(IGameObject* go : goList){
				GameObjectPrio prio = go->GetPriority();
				GameObjectList& goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find( goExecList.begin(),goExecList.end(),go );
				if (it != goExecList.end()) {
					//�폜���X�g���珜�O���ꂽ�B
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManager()) {
						delete (*it);
					}
				}
				goExecList.erase(it);
			}
			goList.clear();
		}
	}
	void CGameObjectManager::Init( int gameObjectPrioMax )
	{
		TK_ASSERT( gameObjectPrioMax <= GAME_OBJECT_PRIO_MAX, "�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔���傫�����܂��B");
		m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}
}