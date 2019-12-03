#pragma once

#include "tkEngine/graphics/tkModel.h"
#include "tkEngine/prefab/tkModelRender.h"
#include "tkEngine/graphics/Dx12/tkSpriteDx12.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start() override;
	void Update() override;
	void ForwardRender(IRenderContext& rc) override ;
	void RenderHUD(IRenderContext& rc) override;
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
	prefab::CModelRender* m_bgModelRender = nullptr;				//背景モデルレンダラー。
	prefab::CModelRender* m_modelRender[enNumModel] = { nullptr };	//モデルレンダラー。
	EnInitStep m_initStep = enInitStep_LoadStart;	//初期化ステップ。
	CVector3 m_position = g_vec3Zero;				//座標。
	CQuaternion m_rotation = g_quatIdentity;		//クォータニオン。
	UPITexture m_texture;							//テクスチャ。
	int m_currentModel = 0;
	prefab::CDirectionLight* m_dirLig;
	//スプライト作成テスト。
	CSpriteDx12 m_sprite;

};

