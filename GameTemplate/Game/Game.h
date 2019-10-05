#pragma once

#include "tkEngine/graphics/tkModel.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void ForwardRender(IRenderContext& renderContext) override final;
private:
	enum EnInitStep {
		enInitStep_LoadStart,	//読み込み開始。
		enInitStep_Loaded,		//読み込み終了。
	};
	EnInitStep m_initStep = enInitStep_LoadStart;	//初期化ステップ。
	CVector3 m_position = CVector3::Zero;
	CQuaternion m_rotation = CQuaternion::Identity;
	CModel m_model;									//モデルクラス。
};

