#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkModel.h"



namespace tkEngine {
	void CModel::LoadTkmFileAsync(const char* filePath)
	{
		m_tkmFile.LoadAsync(filePath);
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
	void CModel::Draw(
		IRenderContext& rc, 
		const CMatrix& mWorld, 
		const CMatrix& mView, 
		const CMatrix& mProj)
	{
		m_meshParts->Draw(rc, mWorld, mView, mProj);
	}
}