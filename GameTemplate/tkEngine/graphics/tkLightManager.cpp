/*!
 * @brief	���C�g�̊Ǘ��ҁB
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
		//���C�g�̐U�蕪���B
		const std::type_info& typeInfo = typeid(*light);
		if (typeInfo == typeid(CPointLight)) {
			//�o�^�ς݂����ׂ�B
			auto findIt = std::find(m_pointLights.begin(), m_pointLights.end(), light);
			if (findIt == m_pointLights.end()) {
				//�V�K�o�^�B
				m_pointLights.push_back(reinterpret_cast<CPointLight*>(light));
			}
			else {
				//���ɓo�^����Ă��B
				TK_LOG("This light was already added");
				return;
			}
		}
		else if (typeInfo == typeid(prefab::CDirectionLight)) {
			//�o�^�ς݂����ׂ�B
			auto findIt = std::find(m_directionLights.begin(), m_directionLights.end(), light);
			if (findIt == m_directionLights.end()) {
				//�V�K�o�^�B
				m_directionLights.push_back(reinterpret_cast<CDirectionLight*>(light));
			}
			else {
				//���ɓo�^����Ă��B
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
		//�f�B���N�V�������C�g�̃X�g���N�`���[�o�b�t�@���X�V�B
		int ligNo = 0;
		for (auto lig : m_directionLights) {
			if (lig->IsActive() == false) {
				//�A�N�e�B�u����Ȃ��z�̓X�L�b�v�B
				continue;
			}
			m_rawDirectionLights[ligNo] = lig->GetRawData();
			ligNo++;
		}
		int numDirLig = ligNo;
		ligNo = 0;
		for (auto lig : m_pointLights) {
			if (lig->IsActive() == false) {
				//�A�N�e�B�u����Ȃ��z�̓X�L�b�v�B
				continue;
			}
			m_rawPointLights[ligNo] = lig->GetRawData();
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			//�|�C���g���C�g�̃f�[�^�̃G���[�`�F�b�N�B
			if (m_rawPointLights[ligNo].attn.y <= 0.0f) {
				TK_WARNING("m_rawPointLights[ligNo].attn.y�̒l���s���ł��B���̒l�͕K��0���傫�Ȓl������悤�ɂ��Ă��������B");
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
		//�_�[�e�B�t���O�͂����ł͂��낳���ɁARender�֐��ł��낷�B
	}
	
	void ILightManager::Render(IRenderContext& renderContext)
	{
		OnRender(renderContext);
	}
#if 0 //todo
	void CLightManager::Render(CRenderContext& renderContext)
	{
		
		//StructuredBuffer���X�V����B
		renderContext.UpdateSubresource(m_directionLightSB, m_rawDirectionLights);
		renderContext.UpdateSubresource(m_pointLightsSB, m_rawPointLights);
		
		renderContext.UpdateSubresource(m_lightParamCB, &m_lightParam);
		//PS�X�e�[�W��t���W�X�^��100�ԖڂɃf�B���N�V�������C�g�̃X�g���N�`���[�o�b�t�@��ݒ肷��B
		renderContext.PSSetShaderResource(enSkinModelSRVReg_DirectionLight, m_directionLightSB.GetSRV());
		//PS�X�e�[�W��t���W�X�^��101�ԖڂɃ|�C���g���C�g�̃X�g���N�`���[�o�b�t�@��ݒ肷��B
		renderContext.PSSetShaderResource(enSkinModelSRVReg_PointLight, m_pointLightsSB.GetSRV());
		//PS�X�e�[�W��t���W�X�^��102�ԖڂɃ^�C�����Ƃ̃|�C���g���C�g�̃C���f�b�N�X���X�g�̃X�g���N�`���[�o�b�t�@��ݒ肷��B
		renderContext.PSSetShaderResource(enSkinModelSRVReg_PointLightListInTile, m_pointLightsInTileSB.GetSRV());
		//PS�X�e�[�W��c���W�X�^��1�ԖڂɃ��C�g�p�̒萔�o�b�t�@��ݒ肷��B
		renderContext.PSSetConstantBuffer(enSkinModelCBReg_Light, m_lightParamCB);
	}
	/*!
	*@brief�@�P�t���[���̕`��I�����ɌĂ΂�鏈���B
	*/
	void CLightManager::EndRender(CRenderContext& renderContext)
	{
		renderContext.PSUnsetShaderResource(100);
		renderContext.PSUnsetShaderResource(101);
		renderContext.PSUnsetShaderResource(102);
	}
#endif
}