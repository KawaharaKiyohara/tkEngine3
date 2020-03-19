/*!
 * @brief	物理オブジェクトのベースクラス。
 */


#include "tkEngine/tkEnginePreCompile.h"
#include "tkPhysicsObjectBase.h"
#include "tkBoxCollider.h"
#include "tkSphereCollider.h"
#include "tkCapsuleCollider.h"
#include "tkMeshCollider.h"
#include "tkEngine/graphics/tkModel.h"
#include "tkEngine/prefab/tkModelRender.h"

using namespace std;
namespace tkEngine{
	
	void CPhysicsObjectBase::CreateBox(CVector3 pos, CQuaternion rot, CVector3 size)
	{
		Release();
		auto boxCollider = make_unique<CBoxCollider>();
		boxCollider->Create(size);
		m_collider = move(boxCollider);
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateCapsule(CVector3 pos, CQuaternion rot, float radius, float height )
	{
		Release();
		auto capusuleCollider = make_unique<CCapsuleCollider>();
		capusuleCollider->Create(radius, height);
		m_collider = move(capusuleCollider);
		CreateCommon(pos, rot);
	}
	
	void CPhysicsObjectBase::CreateSphere(CVector3 pos, CQuaternion rot, float radius)
	{
		Release();
		auto sphereCollider = make_unique<CSphereCollider>();
		sphereCollider->Create(radius);
		m_collider = move(sphereCollider);
		CreateCommon(pos, rot);
	}

	void CPhysicsObjectBase::CreateMesh(CVector3 pos, CQuaternion rot, const CTkmFile& tkmFile)
	{
		Release();
		auto meshCollider = make_unique<CMeshCollider>();
		meshCollider->CreateFromTkmFile(tkmFile, nullptr);
		m_collider = move(meshCollider);
		CreateCommon(pos, rot);
	}
	void CPhysicsObjectBase::CreateMesh(CVector3 pos, CQuaternion rot, CVector3 scale, prefab::CModelRender* modelRender)
	{
		CreateMesh(pos, rot, scale, modelRender->GetModel());
	}
	void CPhysicsObjectBase::CreateMesh(CVector3 pos, CQuaternion rot, CVector3 scale, const CModel& model)
	{
		Release();
		CMatrix mScale;
		mScale.MakeScaling(scale);
		auto meshCollider = make_unique<CMeshCollider>();
		meshCollider->CreateFromModel(model, &mScale);
		m_collider = move(meshCollider);
		CreateCommon(
			pos,
			rot
		);
		
	}
}
