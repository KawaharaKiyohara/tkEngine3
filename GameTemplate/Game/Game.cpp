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
		//読み込み開始ステップ。
		m_model.LoadTkmFileAsync("modelData/test.tkm");
		m_initStep = enInitStep_Loaded;
		break;
	case enInitStep_Loaded:
		if (m_model.IsInited()) {
			//モデルの初期化が終わったのでtrueを返す。
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
}