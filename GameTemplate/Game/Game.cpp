#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}


Game::~Game()
{
}
bool Game::Start()
{
	//ƒJƒƒ‰‚ðÝ’èB
	g_camera3D->SetTarget({ 0.0f, 70.0f, 0.0f });
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 70.0f, 200.0f });
	g_camera3D->Update();
	
	return true;
}

void Game::Update()
{
	if (g_pad[0]->IsPress(enButtonLeft)) {
		TK_LOG("Pad0::Left");
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		TK_LOG("Pad0::Right");
	}
	if (g_pad[0]->IsPress(enButtonUp)) {
		TK_LOG("Pad0::Up");
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		TK_LOG("Pad0::Down");
	}

	if (g_pad[1]->IsPress(enButtonLeft)) {
		TK_LOG("Pad1::Left");
	}
	if (g_pad[1]->IsPress(enButtonRight)) {
		TK_LOG("Pad1::Right");
	}
	if (g_pad[1]->IsPress(enButtonUp)) {
		TK_LOG("Pad1::Up");
	}
	if (g_pad[1]->IsPress(enButtonDown)) {
		TK_LOG("Pad1::Down");
	}

	if (g_pad[2]->IsPress(enButtonLeft)) {
		TK_LOG("Pad2::Left");
	}
	if (g_pad[2]->IsPress(enButtonRight)) {
		TK_LOG("Pad2::Right");
	}
	if (g_pad[2]->IsPress(enButtonUp)) {
		TK_LOG("Pad2::Up");
	}
	if (g_pad[2]->IsPress(enButtonDown)) {
		TK_LOG("Pad2::Down");
	}

	if (g_pad[3]->IsPress(enButtonLeft)) {
		TK_LOG("Pad3::Left");
	}
	if (g_pad[3]->IsPress(enButtonRight)) {
		TK_LOG("Pad3::Right");
	}
	if (g_pad[3]->IsPress(enButtonUp)) {
		TK_LOG("Pad3::Up");
	}
	if (g_pad[3]->IsPress(enButtonDown)) {
		TK_LOG("Pad3::Down");
	}

}