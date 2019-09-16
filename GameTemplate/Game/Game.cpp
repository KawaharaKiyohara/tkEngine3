#include "stdafx.h"
#include "Game.h"
#include "tkEngine/graphics/tkTkmFile.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	switch (m_initStep) {
	case enInitStep_LoadStart:
		//�J������ݒ�B
		g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
		g_camera3D->SetNear(0.1f);
		g_camera3D->SetFar(10000.0f);
		g_camera3D->SetPosition({ 0.0f, 0.0f, -5.0f });
		g_camera3D->Update();
		//�ǂݍ��݊J�n�X�e�b�v�B
		m_model.LoadTkmFileAsync("modelData/test.tkm");
		m_initStep = enInitStep_Loaded;
		break;
	case enInitStep_Loaded:
		if (m_model.IsInited()) {
			//���b�V���p�[�c�̍쐬�B
			m_model.CreateMeshParts();
			//���f���̏��������I������̂�true��Ԃ��B
			return true;
		}
		break;
	}

	return false;
}

void Game::Update()
{

}
void Game::ForwardRender(IRenderContext& renderContext)
{
	m_model.Draw(
		renderContext, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix());
}