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
	g_camera3D->SetTarget({ 0.0f, 00.0f, 0.0f });
	g_camera3D->SetNear(0.1f);
	g_camera3D->SetFar(10000.0f);
	g_camera3D->SetPosition({ 0.0f, 0.0f, -5.0f });
	g_camera3D->Update();
	
	m_triangleShape = Engine().GetGraphicsInstanceFactory()->CreateTriangleShapeInstance();

	return true;
}

void Game::Update()
{
	auto pos = m_triangleShape->GetPosition();
	if (g_pad[0]->IsPress(enButtonLeft)) {
		pos.x -= 0.1f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		pos.x += 0.1f;
	}
	auto rot = m_triangleShape->GetRotation();
	if (g_pad[0]->IsPress(enButtonUp)) {
		rot.AddRotationY(0.03f);
	}else if (g_pad[0]->IsPress(enButtonDown)) {
		rot.AddRotationY(-0.03f);
	}
	m_triangleShape->SetRotation(rot);
	m_triangleShape->SetPosition(pos);
}
void Game::ForwardRender(IRenderContext& renderContext)
{
	m_triangleShape->Update();
	m_triangleShape->Draw(renderContext, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
}