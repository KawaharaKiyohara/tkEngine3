/*!
 * @brief	動的物理オブジェクト。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/physics/tkPhysicsDynamicObject.h"

namespace tkEngine{

	void CPhysicsDynamicObject::Release()
	{
		g_physicsWorld->RemoveRigidBody(m_rigidBody);
	}
	void CPhysicsDynamicObject::CreateCommon(CVector3 pos, CQuaternion rot)
	{
		RigidBodyInfo rbInfo;
		rbInfo.collider = m_collider.get();
		rbInfo.mass = m_mass;
		rbInfo.pos = pos;
		rbInfo.rot = rot;
		rbInfo.localInteria = m_localInterial;
		m_rigidBody.Create(rbInfo);
		g_physicsWorld->AddRigidBody(m_rigidBody);
	}

}