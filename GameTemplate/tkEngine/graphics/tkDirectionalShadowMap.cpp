#include "tkEngine/tkEnginePreCompile.h"
#include "tkDirectionalShadowMap.h"
#include "tkModel.h"

namespace tkEngine {
	CVector3 IDirectionalShadowMap::CalcLightPosition(float lightHeight, CVector3 viewFrustomCenterPosition)
	{
		//計算式の詳細はヘッダーファイルの関数コメントを参照してください。
		auto alpha = (lightHeight - viewFrustomCenterPosition.y) / m_lightDirection.y;
		auto lightPos = viewFrustomCenterPosition + m_lightDirection * alpha;
		return lightPos;
	}
	void IDirectionalShadowMap::Update()
	{
		if (m_isEnable == false) {
			return;
		}
		//シーンのレンダリングに使用しているカメラを使ってライトカメラの回転を求める。
		auto cameraDir = g_camera3D->GetForward();
		if (fabs(cameraDir.x) < FLT_EPSILON && fabsf(cameraDir.z) < FLT_EPSILON) {
			//ほぼ真上をむいている。
			return;
		}
		//ライトビュー行列の回転成分を計算する。
		CVector3 lightViewForward = m_lightDirection;
		CVector3 lightViewUp;
		if (fabsf(lightViewForward.y) > 0.999f) {
			//ほぼ真上。
			lightViewUp.Cross(lightViewForward, CVector3::Right);
		}
		else {
			lightViewUp.Cross(lightViewForward, CVector3::Up);
		}
		lightViewUp.Normalize();
		CVector3 lgihtViewRight;
		lgihtViewRight.Cross(lightViewUp, lightViewForward);
		lgihtViewRight.Normalize();

		CMatrix lightViewRot;
		//ライトビューの横を設定する。
		lightViewRot.m[0][0] = lgihtViewRight.x;
		lightViewRot.m[0][1] = lgihtViewRight.y;
		lightViewRot.m[0][2] = lgihtViewRight.z;
		lightViewRot.m[0][3] = 0.0f;
		//ライトビューの上を設定する。
		lightViewRot.m[1][0] = lightViewUp.x;
		lightViewRot.m[1][1] = lightViewUp.y;
		lightViewRot.m[1][2] = lightViewUp.z;
		lightViewRot.m[1][3] = 0.0f;
		//ライトビューの前を設定する。
		lightViewRot.m[2][0] = lightViewForward.x;
		lightViewRot.m[2][1] = lightViewForward.y;
		lightViewRot.m[2][2] = lightViewForward.z;
		lightViewRot.m[2][3] = 0.0f;

		

		//ライトビューの高さを計算。
		float lightHeight = m_lightHeight;

		SShadowCb shadowCB;
		float nearPlaneZ = 0.0f;
		float farPlaneZ;
		CVector3 cameraUp;
		cameraUp.Cross(g_camera3D->GetRight(), g_camera3D->GetForward());
		//視推台を分割するようにライトビュープロジェクション行列を計算する。
		for (int i = 0; i < NUM_SHADOW_MAP; i++) {
			farPlaneZ = nearPlaneZ + m_shadowAreas[i];
			CMatrix mLightView = CMatrix::Identity;
			float halfViewAngle = g_camera3D->GetViewAngle() * 0.5f;
			//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅と高さを求める。
			float w, h;
			float far_z = -1.0f;
			CVector3 v[8];
			{
				float t = tan(halfViewAngle);
				CVector3 toUpperNear, toUpperFar;
				toUpperNear = cameraUp * t * nearPlaneZ;
				toUpperFar = cameraUp * t * farPlaneZ;
				t *= g_camera3D->GetAspect();
				//近平面の中央座標を計算。
				auto nearPlaneCenterPos = g_camera3D->GetPosition() + cameraDir * nearPlaneZ;
				v[0] = nearPlaneCenterPos + g_camera3D->GetRight() * t * nearPlaneZ + toUpperNear;
				v[1] = v[0] - toUpperNear * 2.0f;

				v[2] = nearPlaneCenterPos + g_camera3D->GetRight() * -t * nearPlaneZ + toUpperNear;
				v[3] = v[2] - toUpperNear * 2.0f;

				//遠平面の中央座標を計算。
				auto farPlaneCneterPos = g_camera3D->GetPosition() + cameraDir * farPlaneZ;
				v[4] = farPlaneCneterPos + g_camera3D->GetRight() * t * farPlaneZ + toUpperFar;
				v[5] = v[4] - toUpperFar * 2.0f;
				v[6] = farPlaneCneterPos + g_camera3D->GetRight() * -t * farPlaneZ + toUpperFar;
				v[7] = v[6] - toUpperFar * 2.0f;

				//ライト行列を作成。
				auto viewFrustumCenterPosition = (nearPlaneCenterPos + farPlaneCneterPos) * 0.5f;
				auto lightPos = CalcLightPosition(lightHeight, viewFrustumCenterPosition);

				mLightView = lightViewRot;

				mLightView.m[3][0] = lightPos.x;
				mLightView.m[3][1] = lightPos.y;
				mLightView.m[3][2] = lightPos.z;
				mLightView.m[3][3] = 1.0f;
				mLightView.Inverse(mLightView);	//ライトビュー完成。
				//視推台を構成する8頂点が計算できたので、ライト空間に座標変換して、AABBを求める。
				CVector3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				CVector3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
				for (auto& vInLight : v) {
					mLightView.Apply(vInLight);
					vMax.Max(vInLight);
					vMin.Min(vInLight);

				}
				w = vMax.x - vMin.x;
				h = vMax.y - vMin.y;
				far_z = vMax.z;
			}
			CMatrix proj;
			proj.MakeOrthoProjectionMatrix(
				w,
				h,
				far_z / 100.0f,
				far_z
			);
			m_LVPMatrix[i] = mLightView * proj;
			m_shadowCbEntity.mLVP[i] = m_LVPMatrix[i];
			m_shadowCbEntity.shadowAreaDepthInViewSpace[i] = farPlaneZ * 0.8f;	//キワキワだと変な境界が発生していたので、ちょっとエリアを狭める。
			nearPlaneZ = farPlaneZ;
		}

	}
}