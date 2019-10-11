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
		enInitStep_LoadStart,	//�ǂݍ��݊J�n�B
		enInitStep_Loaded,		//�ǂݍ��ݏI���B
	};
	prefab::CModelRender* m_modelRender = nullptr;	//���f�������_���[�B
	EnInitStep m_initStep = enInitStep_LoadStart;	//�������X�e�b�v�B
	CVector3 m_position = g_vec3Zero;				//���W�B
	CQuaternion m_rotation = g_quatIdentity;		//�N�H�[�^�j�I���B
};

