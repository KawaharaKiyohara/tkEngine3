#include "tkEngine/tkEnginePreCompile.h"
#include "tkShape.h"


namespace tkEngine{
	
	void IShape::Update()
	{
		UpdateWorldMatrix();
	}
	
	void IShape::UpdateWorldMatrix()
	{
		CMatrix mTrans, mRot, mScale;
		mScale.MakeScaling(m_scale);
		mRot.MakeRotationFromQuaternion(m_rotation);
		mTrans.MakeTranslation(m_position);
		m_worldMatrix = mScale * mRot * mTrans;
	}
}