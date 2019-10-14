/*!
 * @brief	アニメーション
 */

#pragma once

#include "tkEngine/graphics/tkAnimationClip.h"
#include "tkEngine/graphics/tkAnimationPlayController.h"

namespace tkEngine{
	namespace prefab {
		class CSkinModelRender;
	}
	class CSkeleton;
	using AnimationEventListener = std::function<void(const wchar_t* clipName, const wchar_t* eventName)>;
	/*!
	 * @brief	アニメーションクラス。
	 */
	class CAnimation {
	public:

		/// <summary>
		/// 初期化済みか判定。
		/// </summary>
		/// <returns>trueが返ってきたら初期化済み。</returns>
		bool IsInited() const
		{
			return m_isInited;
		}

		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="skeleton">アニメーションさせるスケルトン</param>
		/// <param name="animClips">アニメーションクリップ。</param>
		void Init(CSkeleton& skeleton, const vector<unique_ptr<CAnimationClip>>& animClips);
		/// <summary>
		/// アニメーションの再生。
		/// </summary>
		/// <param name="clipNo">アニメーションクリップの番号。Init関数に渡したanimClipListの並びとなる。</param>
		/// <param name="interpolateTime">補完時間(単位：秒)</param>
		void Play(int clipNo, float interpolateTime = 0.0f)
		{
			PlayCommon(m_animationClips[clipNo], interpolateTime);
		}
		/*!
		*@brief	アニメーションクリップのループフラグを設定します。
		*@param[in]	clipName	アニメーションクリップの名前。
		*@param[in]	flag		フラグ。
		*/
		void SetAnimationClipLoopFlag(const wchar_t* clipName, bool flag)
		{
			auto it = std::find_if(
				m_animationClips.begin(),
				m_animationClips.end(),
				[clipName](auto& clip) {return clip->GetName() == clipName; }
			);
			if (it == m_animationClips.end()) {
				//見つからなかった。
				return;
			}
			(*it)->SetLoopFlag(flag);
		}
		/*!
		* @brief	アニメーションの再生中？
		*/
		bool IsPlaying() const
		{
			int lastIndex = GetLastAnimationControllerIndex();
			return m_animationPlayController[lastIndex].IsPlaying();
		}
		/*!
		 *@brief	フリーズさせているボーンの平行移動量を取得。
		 *@details
		 * 最終ポーズのアニメーションだけ。
		 */
		CVector3 GetFreezeBoneTranslate() const
		{
			int lastIndex = GetLastAnimationControllerIndex();
			return m_animationPlayController[lastIndex].GetFreezeBoneTranslate();
		}
		/*!
		*@brief	アニメーションのポスト処理を実行。
		*@details
		* CSkeleton::Updateから呼ばれている。ユーザーが呼び出す必要はないので、使用しないでください。
		*/
		void PostProcess();
	
		/*!
		* @brief	アニメーションを進める。
		*@details
		* エンジン内部から呼ばれます。
		* ユーザーは使用しないでください。
		*@param[in]	deltaTime		アニメーションを進める時間(単位：秒)。
		*/
		void Update(float deltaTime);
		/*!
		*@brief	アニメーションイベントリスナーを登録。
		*@return
		* 登録されたリスナー。
		*/
		void AddAnimationEventListener(AnimationEventListener eventListener)
		{
			m_animationEventListeners.push_back(eventListener);
		}
	
		/*!
		* @brief	アニメーションイベントをリスナーに通知。
		*/
		void NotifyAnimationEventToListener(const wchar_t* clipName, const wchar_t* eventName)
		{
			for (auto& listener : m_animationEventListeners) {
				listener(clipName, eventName);
			}
		}
	private:
		void PlayCommon(CAnimationClip* nextClip, float interpolateTime)
		{
			if (nextClip->IsLoaded() == false) {
				//アニメーションクリップがロードされていない。
				TK_WARNING_MESSAGE_BOX("警告 : アニメーションクリップがロードされていません。");
				return;
			}
			int index = GetLastAnimationControllerIndex();
			if (m_animationPlayController[index].GetAnimClip() == nextClip) {
				return;
			}
			if (interpolateTime == 0.0f) {
				//補完なし。
				m_numAnimationPlayController = 1;
			}
			else {
				//補完あり。
				m_numAnimationPlayController++;
			}
			index = GetLastAnimationControllerIndex();
			m_animationPlayController[index].ChangeAnimationClip(nextClip);
			m_animationPlayController[index].SetInterpolateTime(interpolateTime);
			m_interpolateTime = 0.0f;
			m_interpolateTimeEnd = interpolateTime;
		}
		/// <summary>
		/// ローカルポーズの更新。
		/// </summary>
		/// <param name="deltaTime">アニメーションを進める時間。単位：秒。</param>
		void UpdateLocalPose(float deltaTime);
		/// <summary>
		/// グローバルポーズの更新。
		/// </summary>
		void UpdateGlobalPose();
		
	private:
		
		/*!
		 *@brief	最終ポーズになるアニメーションのリングバッファ上でのインデックスを取得。
		 */
		int GetLastAnimationControllerIndex() const
		{
			return GetAnimationControllerIndex(m_startAnimationPlayController, m_numAnimationPlayController - 1);
		}
		/*!
		*@brief	アニメーションコントローラのリングバッファ上でのインデックスを取得。
		*@param[in]	startIndex		開始インデックス。
		*@param[in]	localIndex		ローカルインデックス。
		*/
		int GetAnimationControllerIndex(int startIndex, int localIndex) const
		{
			return (startIndex + localIndex) % ANIMATION_PLAY_CONTROLLER_NUM;
		}
	private:
		static const int ANIMATION_PLAY_CONTROLLER_NUM = 32;	//!<アニメーションコントローラの数。
		vector<CAnimationClip*>	m_animationClips;	//!<アニメーションクリップの配列。
		CSkeleton* m_skeleton = nullptr;	//!<アニメーションを適用するスケルトン。
		CAnimationPlayController	m_animationPlayController[ANIMATION_PLAY_CONTROLLER_NUM];	//!<アニメーションコントローラ。リングバッファ。
		int m_numAnimationPlayController = 0;		//!<現在使用中のアニメーション再生コントローラの数。
		int m_startAnimationPlayController = 0;		//!<アニメーションコントローラの開始インデックス。
		float m_interpolateTime = 0.0f;
		float m_interpolateTimeEnd = 0.0f;
		bool m_isInterpolate = false;				//!<補間中？
		vector<AnimationEventListener>	m_animationEventListeners;	//!<アニメーションイベントリスナーのリスト。
		bool m_isInited = false;
	};
}