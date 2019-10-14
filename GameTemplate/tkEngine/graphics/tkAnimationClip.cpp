/*!
 *@brief	アニメーションクリップ。
 */

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/graphics/tkAnimationClip.h"

namespace tkEngine {

	CAnimationClip::~CAnimationClip()
	{
	}

	void CAnimationClip::Load(const char* filePath)
	{
		m_tkaFile.Load(filePath);
		if(m_tkaFile.IsLoaded() == false){
		
			TK_WARNING_MESSAGE_BOX_W(L"アニメーションクリップのオープンに失敗しました。ファイルパス : %s\n"
									 L"原因として、下記の２点が考えられます。\n"
									 L"�@ ファイルパスが間違えている。\n"
									 L"�A Assetsフォルダの中にファイルが存在しない。\n"
									 L"この２点を確認して、問題が存在しない場合は、一度VisualStudioのビルド/リビルドを行ってみてください。\n", filePath);
			
			return;
		}
	}

	void CAnimationClip::BuildKeyFramesAndAnimationEvents()
	{
		//アニメーションイベントの構築。
		auto numAnimEvent = m_tkaFile.GetNumAnimationEvent();
		if (numAnimEvent > 0) {
			m_animationEvent = std::make_unique<CAnimationEvent[]>(numAnimEvent);
			int eventNo = 0;
			m_tkaFile.QueryAnimationEvents([&](const CTkaFile::AnimationEvent & animEvent) {
				static wchar_t wEventName[256];

				mbstowcs(wEventName, animEvent.eventName.c_str(), 255);
				m_animationEvent[eventNo].SetInvokeTime(animEvent.invokeTime);
				m_animationEvent[eventNo].SetEventName(wEventName);
				eventNo++;
				});

		}
		//キーフレーム情報の構築。
		m_keyframes.reserve(m_tkaFile.GetNumKeyFrame());
		m_tkaFile.QueryKeyFrames([&](const CTkaFile::KeyFrame & tkaKeyFrame) {
			auto keyframe = std::make_unique<KeyFrame>();
			keyframe->boneIndex = tkaKeyFrame.boneIndex;
			keyframe->transform = CMatrix::Identity;
			keyframe->time = tkaKeyFrame.time;
			for (auto j = 0; j < 4; j++) {
				keyframe->transform.m[j][0] = tkaKeyFrame.transform[j].x;
				keyframe->transform.m[j][1] = tkaKeyFrame.transform[j].y;
				keyframe->transform.m[j][2] = tkaKeyFrame.transform[j].z;
			}
			m_keyframes.push_back(std::move(keyframe));
			});
		//ボーンインデックスごとのキーフレームの連結リストを作成する。
		m_keyFramePtrListArray.resize(MAX_BONE);
		for (auto& keyframe : m_keyframes) {
			m_keyFramePtrListArray[keyframe->boneIndex].push_back(keyframe.get());
			if (m_topBoneKeyFramList == nullptr) {
				m_topBoneKeyFramList = &m_keyFramePtrListArray[keyframe->boneIndex];
			}
		}
	}
}

