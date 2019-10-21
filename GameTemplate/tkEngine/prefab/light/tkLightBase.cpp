/*!
 * @brief	ライトの基底クラス。
 */

#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/prefab/light/tkLightBase.h"
#include "tkEngine/graphics/tkLightManager.h"

namespace tkEngine{
	namespace prefab {
		bool CLightBase::Start()
		{
			g_lightManager->AddLight(this);
			return StartSub();
		}
		void CLightBase::OnDestroy()
		{
			g_lightManager->RemoveLight(this);
			return OnDestorySub();
		}
	}
}