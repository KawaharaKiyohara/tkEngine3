#pragma once

#include "tkEngine/graphics/tkModel.h"
#include "tkEngine/prefab/tkModelRender.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
private:
	enum EnInitStep {
		enInitStep_LoadStart,	//読み込み開始。
		enInitStep_Loaded,		//読み込み終了。
	};
	enum EnModel {
		enRobo,
		enUnity,
		enNinja,
		enNumModel,
	};
	prefab::CModelRender* m_modelRender[enNumModel] = { nullptr };	//モデルレンダラー。
	EnInitStep m_initStep = enInitStep_LoadStart;	//初期化ステップ。
	CVector3 m_position = g_vec3Zero;				//座標。
	CQuaternion m_rotation = g_quatIdentity;		//クォータニオン。
	int m_currentModel = 0;
};

