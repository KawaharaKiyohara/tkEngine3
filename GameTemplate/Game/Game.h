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
		enInitStep_LoadStart,	//�ǂݍ��݊J�n�B
		enInitStep_Loaded,		//�ǂݍ��ݏI���B
	};
	EnInitStep m_initStep = enInitStep_LoadStart;	//�������X�e�b�v�B
	CVector3 m_position = CVector3::Zero;
	CModel m_model;									//���f���N���X�B
};

