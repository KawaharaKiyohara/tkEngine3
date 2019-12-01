#include "stdafx.h"
#include "Game.h"
#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/prefab/light/tkDirectionLight.h"

#include "tkEngine/graphics/Dx12/tkRenderTargetDx12.h"
#include "tkEngine/graphics/Dx12/tkSpriteDx12.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	auto gfxFactory = g_engine->GetGraphicsInstanceFactory();
	m_texture = gfxFactory->CreateTextureFromDDSFile(L"modelData/utc_all2.DDS");
	m_sprite.Init(m_texture.get(), 128, 128);

	//カメラを設定。
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 200.0f });
	g_camera3D->SetUp(g_vec3AxisY);
	
	g_lightManager->SetAmbientLight({ 0.3f, 0.3f, 0.3f });
	m_dirLig = NewGO<prefab::CDirectionLight>(0);
	
	m_dirLig->SetDirection({ -0.0f, 0.0f, -1.0f });
	m_dirLig->SetColor({ -0.7f, -0.7f, -0.7f, 1.5f });

	m_modelRender[enRobo] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enRobo]->Init(
		"modelData/robo.tkm");
	m_modelRender[enRobo]->SetScale( 12.0f, 12.0f, 12.0f );
//	m_modelRender[enRobo]->SetActiveFlag(false);
	const char* tkaFilePaths[] = {
		"animData/unityChan/idle.tka",
		"animData/unityChan/run.tka",
		"animData/unityChan/walk.tka",
	};
	m_modelRender[enUnity] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enUnity]->Init(
		"modelData/unityChan.tkm",
		tkaFilePaths
	);
	CQuaternion qRot;
	qRot.SetRotationDeg(g_vec3AxisX, 90.0f);
	m_modelRender[enUnity]->SetRotation(qRot);
	m_modelRender[enUnity]->SetScale( 2.0f, 2.0f, 2.0f );
//	m_modelRender[enUnity]->SetActiveFlag(false);

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
	qRot.SetRotationDeg(g_vec3AxisX, 180.0f);
	m_modelRender[enNinja]->SetRotation(qRot);
	m_modelRender[enNinja]->SetScale( 3.0f, 3.0f, 3.0f );
//	m_modelRender[enNinja]->SetActiveFlag(false);

//	m_modelRender[m_currentModel]->SetActiveFlag(true);

	return true;
}

void Game::Update()
{
	CVector3 move = g_vec3Zero;
	move.x = -g_pad[0]->GetLStickXF();
	move.y = g_pad[0]->GetLStickYF();
	CQuaternion qRot;
	if (m_currentModel == enUnity) {
		qRot.SetRotation(g_vec3AxisZ, g_pad[0]->GetRStickXF() * 0.05f);
	}
	else {
		qRot.SetRotation(g_vec3AxisY, g_pad[0]->GetRStickXF() * 0.05f);
	}
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_modelRender[m_currentModel]->PlayAnimation(0, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_modelRender[m_currentModel]->PlayAnimation(1, 0.3f);
	}
	if (g_pad[0]->IsTrigger(enButtonX)) {
		m_modelRender[m_currentModel]->PlayAnimation(2, 0.3f);
	}
	auto dirColor = m_dirLig->GetColor();
	if (g_pad[0]->IsPress(enButtonUp)) {
		dirColor.x += 0.01f;
		dirColor.y += 0.01f;
		dirColor.z += 0.01f;
		m_dirLig->SetColor(dirColor);
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		dirColor.x -= 0.01f;
		dirColor.y -= 0.01f;
		dirColor.z -= 0.01f;
		m_dirLig->SetColor(dirColor);
	}

	m_modelRender[m_currentModel]->Move(move);
	m_modelRender[m_currentModel]->Rotate(qRot);
//	m_modelRender[m_currentModel]->SetActiveFlag(false);
	if (g_pad[0]->IsTrigger(enButtonLeft)) {
		m_currentModel--;
		m_currentModel = max(0, m_currentModel);
	}else if (g_pad[0]->IsTrigger(enButtonRight)) {
		m_currentModel++;
		m_currentModel = min(enNumModel-1, m_currentModel);
	}
//	m_modelRender[m_currentModel]->SetActiveFlag(true);

	static float angle = 0;
	CQuaternion rot;
	angle += 2.0f;
	rot.SetRotationDeg(g_vec3AxisZ, angle);
	m_sprite.Update({-200.0f, 200.0f, 0.0f}, rot, g_vec3One);
}
void Game::ForwardRender(IRenderContext& rc) 
{
}
void Game::RenderHUD(IRenderContext& rc)
{
	m_sprite.Draw(rc, g_camera2D->GetViewMatrix(), g_camera2D->GetProjectionMatrix());
}
