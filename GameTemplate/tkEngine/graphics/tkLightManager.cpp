/*!
 * @brief	ライトの管理者。
 */

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/graphics/tkLightManager.h"
#include "tkEngine/prefab/light/tkPointLight.h"
#include "tkEngine/prefab/light/tkDirectionLight.h"

namespace tkEngine{
	using namespace prefab;
	
	void ILightManager::Init()
	{
		SetAmbientLight({ 0.5f, 0.5f, 0.5f });
		OnInit();
	}
	
	void ILightManager::AddLight(CLightBase* light)
	{
		if (m_pointLights.size() >= MAX_POINT_LIGHT) {
			TK_WARNING("m_pointLights.size() is full!!");
			return;
		}
		if (m_directionLights.size() >= MAX_DIRECTION_LIGHT) {
			TK_WARNING("m_directionLights.size() is full!!");
			return;
		}
		//ライトの振り分け。
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CPointLight)) {
			//登録済みか調べる。
			auto findIt = std::find(m_pointLights.begin(), m_pointLights.end(), light);
			if (findIt == m_pointLights.end()) {
				//新規登録。
				m_pointLights.push_back(reinterpret_cast<CPointLight*>(light));
			}
			else {
				//既に登録されてるよ。
				TK_LOG("This light was already added");
				return;
			}
		}
		else if (typeInfo == typeid(prefab::CDirectionLight)) {
			//登録済みか調べる。
			auto findIt = std::find(m_directionLights.begin(), m_directionLights.end(), light);
			if (findIt == m_directionLights.end()) {
				//新規登録。
				m_directionLights.push_back(reinterpret_cast<CDirectionLight*>(light));
			}
			else {
				//既に登録されてるよ。
				TK_LOG("This light was already added");
				return;
			}
		}
	}
	void ILightManager::RemoveLight(CLightBase* light)
	{
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CPointLight)) {
			m_pointLights.erase(
				std::remove(m_pointLights.begin(), m_pointLights.end(), light), 
				m_pointLights.end()
			);
		}
		else if (typeInfo == typeid(CDirectionLight)) {
			m_directionLights.erase(
				std::remove(m_directionLights.begin(), m_directionLights.end(), light),
				m_directionLights.end()
			);
		}
	}
	void ILightManager::Update()
	{
		m_lightParam.eyePos = g_camera3D->GetPosition();
		//ディレクションライトのストラクチャーバッファを更新。
		int ligNo = 0;
		for (auto lig : m_directionLights) {
			if (lig->IsActive() == false) {
				//アクティブじゃない奴はスキップ。
				continue;
			}
			m_rawDirectionLights[ligNo] = lig->GetRawData();
			ligNo++;
		}
		int numDirLig = ligNo;
		ligNo = 0;
		for (auto lig : m_pointLights) {
			if (lig->IsActive() == false) {
				//アクティブじゃない奴はスキップ。
				continue;
			}
			m_rawPointLights[ligNo] = lig->GetRawData();
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			//ポイントライトのデータのエラーチェック。
			if (m_rawPointLights[ligNo].attn.y <= 0.0f) {
				TK_WARNING("m_rawPointLights[ligNo].attn.yの値が不正です。この値は必ず0より大きな値が入るようにしてください。");
			}
#endif
			ligNo++;
		}
		int numPointLig = ligNo;

		m_lightParam.numDirectionLight = static_cast<int>(numDirLig);
		m_lightParam.numPointLight = static_cast<int>(numPointLig);
		m_lightParam.eyePos = g_camera3D->GetPosition();
		m_lightParam.screenParam.x = 0.0f;
		m_lightParam.screenParam.y = 0.0f;
		m_lightParam.screenParam.z = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
		m_lightParam.screenParam.w = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
		//ダーティフラグはここではおろさずに、Render関数でおろす。
	}
	
	void ILightManager::Render(IRenderContext& renderContext)
	{
		OnRender(renderContext);
	}
#if 0 //todo
	void CLightManager::Render(CRenderContext& renderContext)
	{
		
		//StructuredBufferを更新する。
		renderContext.UpdateSubresource(m_directionLightSB, m_rawDirectionLights);
		renderContext.UpdateSubresource(m_pointLightsSB, m_rawPointLights);
		
		renderContext.UpdateSubresource(m_lightParamCB, &m_lightParam);
		//PSステージのtレジスタの100番目にディレクションライトのストラクチャーバッファを設定する。
		renderContext.PSSetShaderResource(enSkinModelSRVReg_DirectionLight, m_directionLightSB.GetSRV());
		//PSステージのtレジスタの101番目にポイントライトのストラクチャーバッファを設定する。
		renderContext.PSSetShaderResource(enSkinModelSRVReg_PointLight, m_pointLightsSB.GetSRV());
		//PSステージのtレジスタの102番目にタイルごとのポイントライトのインデックスリストのストラクチャーバッファを設定する。
		renderContext.PSSetShaderResource(enSkinModelSRVReg_PointLightListInTile, m_pointLightsInTileSB.GetSRV());
		//PSステージのcレジスタの1番目にライト用の定数バッファを設定する。
		renderContext.PSSetConstantBuffer(enSkinModelCBReg_Light, m_lightParamCB);
	}
	/*!
	*@brief　１フレームの描画終了時に呼ばれる処理。
	*/
	void CLightManager::EndRender(CRenderContext& renderContext)
	{
		renderContext.PSUnsetShaderResource(100);
		renderContext.PSUnsetShaderResource(101);
		renderContext.PSUnsetShaderResource(102);
	}
#endif
}