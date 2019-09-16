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
		if (m_tkmFile.IsLoaded()) {
			TK_WARNING_MESSAGE_BOX("���̊֐���tkm�t�@�C���̃��[�h���������Ă���Ăяo���Ă��������B");
			return;
		}

		//���b�V���p�[�c�̍쐬�B
		auto factory = Engine().GetGraphicsInstanceFactory();
		m_meshParts = factory->CreateMeshPartsFromTkmFile(m_tkmFile);

	}
}