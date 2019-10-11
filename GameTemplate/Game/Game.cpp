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
	//カメラを設定。
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, -200.0f, 0.0f });
	g_camera3D->SetUp(g_vec3AxisZ);
	g_camera3D->Update();
	//読み込み開始ステップ。
	m_modelRender = NewGO<prefab::CModelRender>(0);
	m_modelRender->Init("modelData/test.tkm");
	
	return true;
}

void Game::Update()
{
	CVector3 move = g_vec3Zero;
	move.x = g_pad[0]->GetLStickXF();
	move.z = g_pad[0]->GetLStickYF();
	CQuaternion qRot;
	qRot.SetRotation(g_vec3AxisZ, g_pad[0]->GetRStickXF() * 0.05f);
	
	m_modelRender->Move(move);
	m_modelRender->Rotate(qRot);
}
