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
	//カメラを設定。
	g_camera3D->SetTarget({ 0.0f, 70.0f, 0.0f });
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 70.0f, 200.0f });
	g_camera3D->Update();
	
	return true;
}

void Game::Update()
{
}