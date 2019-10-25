#include "tkEngine/tkEnginePrecompile.h"
#include "tkSprite.h"


namespace tkEngine {
	const CVector2	ISprite::DEFAULT_PIVOT = { 0.5f, 0.5f };
	void ISprite::Init(ITexture* texture, float w, float h)
	{
		OnInit(texture, w, h);
	}
	void ISprite::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot)
	{
		OnUpdate(pos, rot, scale, pivot);
	}
	void ISprite::Draw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		OnDraw(renderContext, viewMatrix, projMatrix);
	}

}