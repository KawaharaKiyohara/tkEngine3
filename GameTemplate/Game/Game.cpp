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
	//カメラを設定。
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 0.0f, 200.0f });
	g_camera3D->SetUp(g_vec3AxisY);
	g_camera3D->Update();
	//モデルレンダーを作成。。
	m_modelRender = NewGO<prefab::CModelRender>(0);
	g_lightManager->SetAmbientLight({ 0.2f, 0.2f, 0.2f });
	auto lig = NewGO<prefab::CDirectionLight>(0);
	lig->SetDirection({ 1.0f, 0.0f, 0.0f });
	lig->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

	lig = NewGO<prefab::CDirectionLight>(0);
	lig->SetDirection({ -0.707f, 0.0f, 0.707f });
	lig->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

#if 1
	const char* tkaFilePaths[] = {
		"animData/unityChan/idle.tka",
		"animData/unityChan/run.tka",
		"animData/unityChan/walk.tka",
	};
	m_modelRender->Init(
		"modelData/unityChan.tkm",
		tkaFilePaths
	);
	CQuaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisX, -90.0f);
	m_modelRender->SetRotation(qRot);
#else
	const char* tkaFilePaths[] = {
		"animData/thief/idle.tka",
		"animData/thief/run.tka",
		"animData/thief/walk.tka",
	};
	m_modelRender->Init(
		"modelData/Thethief_H.tkm",
		tkaFilePaths
	);
	m_modelRender->SetScale({ 3.0f, 3.0f, 3.0f });
#endif
	return true;
}

void Game::Update()
{
	CVector3 move = g_vec3Zero;
	move.x = -g_pad[0]->GetLStickXF();
	move.y = g_pad[0]->GetLStickYF();
	CQuaternion qRot;
	qRot.SetRotation(g_vec3AxisZ, g_pad[0]->GetRStickXF() * 0.05f);
	
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_modelRender->PlayAnimation(0, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_modelRender->PlayAnimation(1, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonX)) {
		m_modelRender->PlayAnimation(2, 0.3f);
	}
	
	m_modelRender->Move(move);
	m_modelRender->Rotate(qRot);
}
