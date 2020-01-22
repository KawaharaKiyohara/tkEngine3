/*!
 *@brief	マップチップレンダラー
 */
#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/level/tkMapChipRender.h"
#include "tkEngine/level/tkLevel.h"

namespace tkEngine {
	CMapChipRender::CMapChipRender()
	{
	}
	CMapChipRender::~CMapChipRender()
	{
	}
	
	void CMapChipRender::OnDestroy()
	{
		DeleteGO(m_modelRender);
	}
	
	void CMapChipRender::Update()
	{
		//if (m_renderObjDatas.size() > 1) {
		//	for (auto& objData : m_renderObjDatas) {
		//		m_modelRender->UpdateInstancingData(objData.position, objData.rotation, objData.scale);
		//	}
		//}
	}

	void CMapChipRender::InitAfterAddAllRenderObjects()
	{
		//スキンモデルレンダラーを初期化する。
		m_modelRender = NewGO<prefab::CModelRender>(0);
		//ファイルパスを作成。
		wchar_t filePath[256];
		swprintf_s(filePath, L"modelData/%s.tkm", m_renderObjDatas[0].name);
		//if (m_renderObjDatas.size() > 1) {
			//インスタンシング
			//m_modelRender->Init(filePath, nullptr, 0, enFbxUpAxisZ, m_renderObjDatas.size());
		//}
		//else {
			//のっとインスタンシング
		size_t origsize = wcslen(filePath) + 1;
		size_t convertedChars = 0;
		char strConcat[] = "";
		size_t strConcatsize = (strlen(strConcat) + 1) * 2;
		const size_t newsize = origsize * 2;
		char* nstring = new char[newsize + strConcatsize];
		wcstombs_s(&convertedChars, nstring, newsize, filePath, _TRUNCATE);
		_mbscat_s((unsigned char*)nstring, newsize + strConcatsize, (unsigned char*)strConcat);
		m_modelRender->Init(nstring);
		m_modelRender->SetPosition(m_renderObjDatas[0].position);
		m_modelRender->SetRotation(m_renderObjDatas[0].rotation);
		m_modelRender->SetScale(m_renderObjDatas[0].scale);
		//}
		////影
		//m_modelRender->SetShadowReceiverFlag(true);
		//m_modelRender->SetShadowCasterFlag(true);
	}
}
