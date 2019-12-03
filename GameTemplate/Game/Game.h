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
		enInitStep_LoadStart,	//�ǂݍ��݊J�n�B
		enInitStep_Loaded,		//�ǂݍ��ݏI���B
	};
	enum EnModel {
		enRobo,
		enUnity,
		enNinja,
		enNumModel,
	};
	prefab::CModelRender* m_bgModelRender = nullptr;				//�w�i���f�������_���[�B
	prefab::CModelRender* m_modelRender[enNumModel] = { nullptr };	//���f�������_���[�B
	EnInitStep m_initStep = enInitStep_LoadStart;	//�������X�e�b�v�B
	CVector3 m_position = g_vec3Zero;				//���W�B
	CQuaternion m_rotation = g_quatIdentity;		//�N�H�[�^�j�I���B
	UPITexture m_texture;							//�e�N�X�`���B
	int m_currentModel = 0;
	prefab::CDirectionLight* m_dirLig;
	//�X�v���C�g�쐬�e�X�g�B
	CSpriteDx12 m_sprite;

};

