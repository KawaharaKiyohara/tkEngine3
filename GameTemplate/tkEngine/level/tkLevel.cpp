/*!
 *@brief	レベル。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/level/tkLevel.h"
#include "tkEngine/level/tkMapChip.h"
#include "tkEngine/level/tkMapChipRender.h"
#include <comdef.h> 

namespace tkEngine{
	CLevel::~CLevel()
	{
		for (auto mapChipRender : m_mapChipRenderPtrs) {
			DeleteGO(mapChipRender.second);
		}
	}
	CMapChipRender* CLevel::CreateMapChipRenderOrAddRenderObject(const LevelObjectData& objData)
	{
		WNameKey nameKey(objData.name);

		auto itFind = m_mapChipRenderPtrs.find(nameKey.GetHashCode());
		CMapChipRender* pMapChipRender = nullptr;
		if (itFind == m_mapChipRenderPtrs.end()) {
			//登録されていないオブジェクト。
			auto mapChipRender = NewGO<CMapChipRender>(0);
			pMapChipRender = mapChipRender;
			m_mapChipRenderPtrs.insert({ nameKey.GetHashCode(),mapChipRender });
		}
		else {
			//描画すべきオブジェクトのインクリメント。
			pMapChipRender = itFind->second;
		}
		pMapChipRender->AddRenderObject(objData);
		return pMapChipRender;
	}
	void CLevel::Init( 
		const wchar_t* filePath, 
		std::function<bool(LevelObjectData& objData)> hookFunc
	)
	{
		//スケルトンをロードする。
		size_t origsize = wcslen(filePath) + 1;
		size_t convertedChars = 0;
		char strConcat[] = "";
		size_t strConcatsize = (strlen(strConcat) + 1) * 2;
		const size_t newsize = origsize * 2;
		char* nstring = new char[newsize + strConcatsize];
		wcstombs_s(&convertedChars, nstring, newsize, filePath, _TRUNCATE);
		_mbscat_s((unsigned char*)nstring, newsize + strConcatsize, (unsigned char*)strConcat);

		//レベルをロードする
		m_tklFile.Load(nstring);
		BuildBoneMatrices();

		for (int i = 1; i < m_bones.size(); i++) {
			auto bone = m_bones[i].get();
			if (bone->GetParentId() == 0) {	//親がルートの場合だけマップチップを生成する。
				LevelObjectData objData;
				CVector3 scale;
				bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);
				//3dsMaxとは軸が違うので、補正を入れる。
				auto t = objData.position.y;
				objData.position.y = objData.position.z;
				objData.position.z = -t;

				t = objData.rotation.y;
				objData.rotation.y = objData.rotation.z;
				objData.rotation.z = -t;
				objData.name = bone->GetName();
				std::swap(objData.scale.y, objData.scale.z);

				auto isHook = false;
				if (hookFunc != nullptr) {
					//hook関数が指定されているのでhook関数を呼び出す。
					isHook = hookFunc(objData);
				}
				if (isHook == false) {
					//マップチップレンダラーを作成する。
					CreateMapChipRenderOrAddRenderObject(objData);
				}
			}
		}

		//マップチップレンダラーを初期化する。
		for (auto& mapChipRender : m_mapChipRenderPtrs) {
			mapChipRender.second->InitAfterAddAllRenderObjects();
			mapChipRender.second->QueryRenderObjDatas([&](const LevelObjectData& objData) {
				//フックされなかったので、マップチップを作成する。
				auto mapChip = std::make_unique<CMapChip>(objData, mapChipRender.second);
				m_mapChipPtrs.push_back(std::move(mapChip));
			});
		}
	}	


	void CLevel::BuildBoneMatrices()
	{
		m_tklFile.QueryObject([&](CTklFile::SObject& tklObj) {
			//バインドポーズ。
			CMatrix bindPoseMatrix;
			memcpy(bindPoseMatrix.m[0], &tklObj.bindPose[0], sizeof(tklObj.bindPose[0]));
			memcpy(bindPoseMatrix.m[1], &tklObj.bindPose[1], sizeof(tklObj.bindPose[1]));
			memcpy(bindPoseMatrix.m[2], &tklObj.bindPose[2], sizeof(tklObj.bindPose[2]));
			memcpy(bindPoseMatrix.m[3], &tklObj.bindPose[3], sizeof(tklObj.bindPose[3]));
			bindPoseMatrix.m[0][3] = 0.0f;
			bindPoseMatrix.m[1][3] = 0.0f;
			bindPoseMatrix.m[2][3] = 0.0f;
			bindPoseMatrix.m[3][3] = 1.0f;

			//バインドポーズの逆行列。
			CMatrix invBindPoseMatrix;
			memcpy(invBindPoseMatrix.m[0], &tklObj.invBindPose[0], sizeof(tklObj.invBindPose[0]));
			memcpy(invBindPoseMatrix.m[1], &tklObj.invBindPose[1], sizeof(tklObj.invBindPose[1]));
			memcpy(invBindPoseMatrix.m[2], &tklObj.invBindPose[2], sizeof(tklObj.invBindPose[2]));
			memcpy(invBindPoseMatrix.m[3], &tklObj.invBindPose[3], sizeof(tklObj.invBindPose[3]));
			invBindPoseMatrix.m[0][3] = 0.0f;
			invBindPoseMatrix.m[1][3] = 0.0f;
			invBindPoseMatrix.m[2][3] = 0.0f;
			invBindPoseMatrix.m[3][3] = 1.0f;

			wchar_t boneName[256];
			mbstowcs(boneName, tklObj.name.get(), 256);
			CBonePtr bone = std::make_unique<CBone>(
				boneName,
				bindPoseMatrix,
				invBindPoseMatrix,
				tklObj.parentNo,
				tklObj.no
				);
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			//ボーンのバリデーションチェック。
			auto it = std::find_if(m_bones.begin(), m_bones.end(), [&](auto& bone) {return wcscmp(boneName, bone->GetName()) == 0;  });
			if (it != m_bones.end()) {
				//同名のボーンが見つかった。
				_bstr_t b(boneName);
				const char* c = b;
				TK_WARNING("同名のボーンが見つかりました。未定の動作です。データを修正してください。%s", c);
			}
#endif
			m_bones.push_back(std::move(bone));
			});
		for (auto& bone : m_bones) {
			if (bone->GetParentId() != -1) {
				m_bones.at(bone->GetParentId())->AddChild(bone.get());
				//ローカルマトリクスを計算。
				const CMatrix& parentMatrix = m_bones.at(bone->GetParentId())->GetInvBindPoseMatrix();
				CMatrix localMatrix;
				localMatrix = bone->GetBindPoseMatrix() * parentMatrix;
				bone->SetLocalMatrix(localMatrix);
			}
			else {
				bone->SetLocalMatrix(bone->GetBindPoseMatrix());
			}
		}

		//ボーン行列を確保
		m_boneMatrixs = std::make_unique<CMatrix[]>(m_bones.size());
		m_isInited = true;
	}
}
