/*!
 * @brief	アニメーション再生コントローラ。
 */

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/graphics/tkAnimationPlayController.h"


namespace tkEngine{
	
	
	void CAnimationPlayController::Init(CSkeleton* skeleton)
	{
		int numBones = skeleton->GetNumBones();
		//ボーン行列をバシッと確保。
		m_boneMatrix.resize(numBones);
		m_skeleton = skeleton;
	}
	
	void CAnimationPlayController::InvokeAnimationEvent(CAnimation* animation)
	{
		auto& animEventArray = m_animationClip->GetAnimationEvent();
		for( auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++){
			if (m_time > animEventArray[i].GetInvokeTime()
				&& animEventArray[i].IsInvoked() == false ) {
				//アニメーションの起動時間を過ぎている且つ、まだイベント起動していない。
				animation->NotifyAnimationEventToListener(
					m_animationClip->GetName(), animEventArray[i].GetEventName()
				);
				animEventArray[i].SetInvokedFlag(true);
			}
		}
	}
	
	void CAnimationPlayController::StartLoop()
	{
		m_currentKeyFrameNo = 0;
		m_time = 0.0f;
		//アニメーションイベントをすべて未発生にする。
		auto& animEventArray = m_animationClip->GetAnimationEvent();
		for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
			animEventArray[i].SetInvokedFlag(false);
		}
	}
	void CAnimationPlayController::Update(float deltaTime, CAnimation* animation)
	{
		if(m_animationClip == nullptr){
			TK_WARNING("m_animationClip is nullptr\n");
			return ;
		}
		const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
		m_time += deltaTime;

		//アニメーションイベントの発生。
		InvokeAnimationEvent(animation);
		//補完時間も進めていく。
		m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
		while (true) {
			if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
				//終端まで行った。
				if (m_animationClip->IsLoop()) {
					//ループ。
					StartLoop();
				}
				else {
					//ワンショット再生。
					m_currentKeyFrameNo--;
					m_isPlaying = false;	//再生終わり。
				}
				break;
			}
			if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
				//終わり。
				break;
			}
			//次へ。
			m_currentKeyFrameNo++;
		}
		//ボーン行列を計算していく。
		const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
		for (const auto& keyFrameList : keyFramePtrListArray) {
			if (keyFrameList.size() == 0) {
				continue;
			}
			//現在再生中のキーフレームを取ってくる。
			KeyFrame* keyframe = keyFrameList.at(m_currentKeyFrameNo);
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
	}
	
}