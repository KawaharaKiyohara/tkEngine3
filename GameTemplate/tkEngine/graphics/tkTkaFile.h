/// <summary>
/// tkaファイル
/// </summary>
/// <remarks>
/// tkaファイルはtkEngine独自のアニメーションクリップフォーマットです。
/// 本クラスはtkaファイルのローダーです。
/// </remarks>
#pragma once

namespace tkEngine {
	class CTkaFile : public IResource{
	public:
		/// <summary>
		/// アニメーションクリップのヘッダー。
		/// </summary>
		struct AnimClipHeader {
			uint32_t numKey;				//キーフレームの数。
			uint32_t numAnimationEvent;	//アニメーションイベントの数。
		};
		/// <summary>
		/// アニメーションイベント。
		/// </summary>
		struct AnimationEvent {
			float	invokeTime;					//アニメーションイベントが発生する時間(単位:秒)
			string eventName;
		};

		/// <summary>
		/// キーフレーム。
		/// </summary>
		struct KeyFrame {
			uint32_t boneIndex;	//ボーンインデックス。
			float time;					//時間。
			CVector3 transform[4];		//トランスフォーム。
		};
		/// <summary>
		/// 読み込み処理。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadImplement(const char* filePath) override final;
		/// <summary>
		/// アニメーションイベントにクエリを行う。
		/// </summary>
		/// <param name="query">クエリ関数</param>
		void QueryAnimationEvents(std::function<void(const AnimationEvent& animEvent) > query)
		{
			for (auto& animEvent : m_animationEvents) {
				query(animEvent);
			}
		}
		/// <summary>
		/// キーフレームにクエリを行う。
		/// </summary>
		/// <param name="query">クエリ関数。</param>
		void QueryKeyFrames(std::function<void(const KeyFrame& keyFrame)> query)
		{
			for (auto& keyFrame : m_keyFrames) {
				query(keyFrame);
			}
		}
		/// <summary>
		/// アニメーションイベントの数を取得。
		/// </summary>
		/// <returns></returns>
		int GetNumAnimationEvent() const
		{
			return static_cast<int>(m_animationEvents.size());
		}
		/// <summary>
		/// キーフレームの数を取得。
		/// </summary>
		/// <returns></returns>
		int GetNumKeyFrame() const
		{
			return static_cast<int>(m_keyFrames.size());
		}
	private:
		vector< AnimationEvent> m_animationEvents;	//アニメーションイベント。
		vector< KeyFrame> m_keyFrames;				//キーフレーム。
	};
}
