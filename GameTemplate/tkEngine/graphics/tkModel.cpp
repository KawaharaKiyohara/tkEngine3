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
			TK_WARNING_MESSAGE_BOX("この関数はtkmファイルのロードが完了してから呼び出してください。");
			return;
		}
		
		//メッシュパーツの作成。
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