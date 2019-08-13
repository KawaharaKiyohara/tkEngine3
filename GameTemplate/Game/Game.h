#pragma once

#include "tkEngine/graphics/Dx12/tkTriangleShapeDx12.h"

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void ForwardRender(IRenderContext& renderContext) override final;
private:
	CTriangleShape m_triangleShape;
};

