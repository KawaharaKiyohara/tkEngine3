#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkModel.h"



namespace tkEngine {
	void CModel::LoadTkmFileAsync(const char* filePath)
	{
		m_tkmFile.LoadAsync(filePath);
	}
	void CModel::LoadTkmFile(const char* filePath)
	{
		m_tkmFile.Load(filePath);
	}
	bool CModel::IsInited() const
	{
		return m_tkmFile.IsLoaded();
	}
	void CModel::CreateMeshParts()
	{
		if (m_tkmFile.IsLoaded() == false) {
			TK_WARNING_MESSAGE_BOX("���̊֐���tkm�t�@�C���̃��[�h���������Ă���Ăяo���Ă��������B");
			return;
		}
		
		//���b�V���p�[�c�̍쐬�B
		auto factory = Engine().GetGraphicsInstanceFactory();
		m_meshParts = factory->CreateMeshPartsFromTkmFile(m_tkmFile);
	}
	void CModel::Update(CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mScale * mRot * mTrans;
	}
	void CModel::Draw(
		IRenderContext& rc, 
		CMatrix mView, 
		CMatrix mProj)
	{
		m_meshParts->Draw(rc, m_world, mView, mProj);
	}
}