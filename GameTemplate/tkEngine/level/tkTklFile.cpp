#include "tkEngine/tkEnginePreCompile.h"
#include "tkTklFile.h"

namespace tkEngine {
	void CTklFile::LoadImplement(const char* filePath)
	{
		auto fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			return;
		}
		//オブジェクトの数を取得。
		fread(&m_numObject, sizeof(m_numObject), 1, fp);
		m_objects.resize(m_numObject);
		for (int i = 0; i < m_numObject; i++) {
			auto& obj = m_objects.at(i);
			size_t nameCount = 0;
			//オブジェクトの名前を取得。
			fread(&nameCount, 1, 1, fp);
			obj.name = std::make_unique<char[]>(nameCount + 1);
			fread(obj.name.get(), nameCount + 1, 1, fp);
			//親のIDを取得。
			fread(&obj.parentNo, sizeof(obj.parentNo), 1, fp);
			//バインドポーズを取得。
			fread(obj.bindPose, sizeof(obj.bindPose), 1, fp);
			//バインドポーズの逆数を取得。
			fread(obj.invBindPose, sizeof(obj.invBindPose), 1, fp);
			//オブジェクトの番号。
			obj.no = i;
			//シャドウキャスターのフラグ。
			fread(&obj.isShadowCaster, sizeof(obj.isShadowCaster), 1, fp);
			//シャドウレシーバーのフラグ。
			fread(&obj.isShadowReceiver, sizeof(obj.isShadowReceiver), 1, fp);
		}

		fclose(fp);
		//読み込み完了の印。
		SetLoadedMark();
	}
}