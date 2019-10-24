#include "stdafx.h"
#include "Game.h"
#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/prefab/light/tkDirectionLight.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ƒJƒƒ‰‚ðÝ’èB
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 200.0f });
	g_camera3D->SetUp(g_vec3AxisY);
	g_camera3D->Update();
		g_lightManager->SetAmbientLight({ 0.3f, 0.3f, 0.3f });
	auto lig = NewGO<prefab::CDirectionLight>(0);
	
	lig->SetDirection({ -1.0f, 0.0f, 0.0f });
	lig->SetColor({ 4.0f, 4.0f, 4.0f, 1.0f });
	
	m_modelRender[enRobo] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enRobo]->Init(
		"modelData/robo.tkm");
	m_modelRender[enRobo]->SetScale( 12.0f, 12.0f, 12.0f );
	m_modelRender[enRobo]->SetActiveFlag(false);
	const char* tkaFilePaths[] = {
		"animData/unityChan/idle.tka",
		"animData/unityChan/run.tka",
		"animData/unityChan/walk.tka",
	};
	m_modelRender[enUnity] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enUnity]->Init(
		"modelData/unityChan.tkm"/*,
		tkaFilePaths*/
	);
	m_modelRender[enUnity]->SetScale( 2.0f, 2.0f, 2.0f );
	m_modelRender[enUnity]->SetActiveFlag(false);

	const char* ninjaTkaFilePaths[] = {
		"animData/thief/idle.tka",
		"animData/thief/run.tka",
		"animData/thief/walk.tka",
	};
	m_modelRender[enNinja] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enNinja]->Init(
		"modelData/Thethief_H.tkm",
		ninjaTkaFilePaths
	);
	m_modelRender[enNinja]->SetScale( 8.0f, 8.0f, 8.0f );
	m_modelRender[enNinja]->SetActiveFlag(false);

	m_modelRender[m_currentModel]->SetActiveFlag(true);

	return true;
}

void Game::Update()
{
	CVector3 move = g_vec3Zero;
	move.x = -g_pad[0]->GetLStickXF();
	move.y = g_pad[0]->GetLStickYF();
	CQuaternion qRot;
	qRot.SetRotation(g_vec3AxisY, g_pad[0]->GetRStickXF() * 0.05f);

	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_modelRender[m_currentModel]->PlayAnimation(0, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_modelRender[m_currentModel]->PlayAnimation(1, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonX)) {
		m_modelRender[m_currentModel]->PlayAnimation(2, 0.3f);
	}
	
	m_modelRender[m_currentModel]->Move(move);
	m_modelRender[m_currentModel]->Rotate(qRot);
	m_modelRender[m_currentModel]->SetActiveFlag(false);
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		m_currentModel--;
		m_currentModel = max(0, m_currentModel);
	}else if (g_pad[0]->IsTrigger(enButtonRight)) {
		m_currentModel++;
		m_currentModel = min(enNumModel-1, m_currentModel);
	}
	m_modelRender[m_currentModel]->SetActiveFlag(true);
}
