#include "stdafx.h"
#include "Game.h"
#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/prefab/light/tkDirectionLight.h"

#include "tkEngine/graphics/Dx12/tkRenderTargetDx12.h"
#include "tkEngine/graphics/Dx12/tkSpriteDx12.h"

#define USE_KIYOHARA
//#define USE_WATANABE

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

	//ƒJƒƒ‰‚ðÝ’èB
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 300.0f, 500.0f });
	g_camera3D->SetUp(g_vec3AxisY);
	
	m_dirLig = NewGO<prefab::CDirectionLight>(0);
	
	m_dirLig->SetDirection({ -1.0f, -1.0f, -0.0f });
	m_dirLig->SetColor({ 0.5f, 0.5f, 0.5f, 1.5f });

	m_dirLig = NewGO<prefab::CDirectionLight>(0);

	m_dirLig->SetDirection({ 0.0f, -1.0f, -1.0f });
	m_dirLig->SetColor({ 0.5f, 0.5f, 0.5f, 1.5f });

	g_lightManager->SetAmbientLight({ 0.4f, 0.4f, 0.4f });
#ifdef USE_KIYOHARA
	m_bgModelRender = NewGO<prefab::CModelRender>(0);
	m_bgModelRender->Init("modelData/bg/bg.tkm");
	m_bgModelRender->SetScale({ 2.0f, 2.0f, 2.0f });

	const char* ninjaTkaFilePaths[] = {
		"animData/thief/walk.tka",
	};
	m_modelRender[enNinja] = NewGO<prefab::CModelRender>(0);
	m_modelRender[enNinja]->Init(
		"modelData/Thethief_H.tkm",
		ninjaTkaFilePaths
	);
	m_modelRender[enNinja]->SetScale( 10.0f, 10.0f, 10.0f );
	
#endif
#ifdef USE_WATANABE
	m_level.Init("level/test.tkl", [&](LevelObjectData& objData) {
		return false;
	});
#endif
	return true;
}

void Game::Update()
{
#ifdef USE_KIYOHARA
	CVector3 move = g_vec3Zero;
	move.x = -g_pad[0]->GetLStickXF();
	move.y = g_pad[0]->GetLStickYF();

	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_modelRender[enNinja]->PlayAnimation(0, 0.3f);
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

	auto footstepDeltaValue = m_modelRender[enNinja]->GetFootstepBoneDeltaValue();
	move.x += footstepDeltaValue.x;
	move.z += footstepDeltaValue.z;
	m_modelRender[enNinja]->Move(move);

	//ƒJƒƒ‰’Ç]
	auto target = m_modelRender[enNinja]->GetPosition();
	target.y += 200.0f;
	g_camera3D->SetTarget(target);
	auto pos = target;
	pos.z += 300.0f;
	g_camera3D->SetPosition(pos);
#endif
}
void Game::ForwardRender(IRenderContext& rc) 
{
}
void Game::RenderHUD(IRenderContext& rc)
{
}
