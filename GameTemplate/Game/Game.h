#pragma once


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void ForwardRender(IRenderContext& renderContext) override final;
private:
	UPIShape m_triangleShape;
};

