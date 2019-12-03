/*!
 *@brief	ディレクションライト。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/prefab/light/tkDirectionLight.h"

namespace tkEngine{
	namespace prefab {
		CDirectionLight::CDirectionLight()
		{
			m_light.color = CVector3::One;
			m_light.direction = CVector3::Down;
		}
		CDirectionLight::~CDirectionLight()
		{

		}
		bool CDirectionLight::StartSub()
		{
			return true;
		}
		void CDirectionLight::Update()
		{
			m_light.lightingMaterialIDGroup = GetLightingMaterialIDGroup();
		}
		void CDirectionLight::SetDirection( CVector3 direction)
		{
			direction.Normalize();
			m_light.direction = direction;
		}
		
		void CDirectionLight::SetColor(const CVector4& color)
		{
			m_light.color = color;
		}
	}
}