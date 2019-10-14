/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

#include "tkEngine/graphics/tkTkaFile.h"

namespace tkEngine{
	
	/*!
	 *@brief	キーフレーム。
	 */
	struct KeyFrame {
		uint32_t boneIndex;	//!<ボーンインデックス。
		float time;					//!<時間。
		CMatrix transform;			//!<トランスフォーム。
	};
	
	/*!
	*@brief	アニメーションイベント。
	*/
	class CAnimationEvent : Noncopyable {
	public:
		
		/*!
		*@brief	イベント発生時間を設定。
		*/
		float GetInvokeTime() const
		{
			return m_invokeTime;
		}
		/*!
		*@brief	イベント名を取得。
		*/
		const wchar_t* GetEventName() const
		{
			return m_eventName.c_str();
		}
		/*!
		*@brief	イベント発生時間を設定。
		*/
		void SetInvokeTime(float time)
		{
			m_invokeTime = time;
		}
		/*!
		*@brief	イベント名を設定。
		*/
		void SetEventName(const wchar_t* name)
		{
			m_eventName = name;
		}
		/*!
		*@brief	イベントが発生済みか判定。
		*/
		bool IsInvoked() const
		{
			return m_isInvoked;
		}
		/*!
		*@brief	イベントが発生済みのフラグを設定する。
		*/
		void SetInvokedFlag(bool flag)
		{
			m_isInvoked = flag;
		}
	private:
		bool m_isInvoked = false;	//!<イベント発生済み？
		float m_invokeTime = 0.0f;	//!<イベント発生時間。
		wstring m_eventName;	//!<イベント名。
	};
	/*!
	 *@brief	アニメーションクリップ。
	 */
	class CAnimationClip : Noncopyable {
	public:
		/*!
		 *@brief	ボーンのフリーズ情報。
		 */
		struct SFreezeBoneInfo {
			int boneNo;		//!<フリーズさせるボーン番号。
			bool freezeX;	//!<X方向の移動をフリーズさせる？
			bool freezeY;	//!<Y方向の移動をフリーズさせる？
			bool freezeZ;	//!<Z方向の移動をフリーズさせる？
		};
		using keyFramePtrList = vector<KeyFrame*>;
		/*!
		* @brief	コンストラクタ
		*/
		CAnimationClip()
		{
			m_freezeBoneInfo.boneNo = -1;
			m_freezeBoneInfo.freezeX = false;
			m_freezeBoneInfo.freezeY = false;
			m_freezeBoneInfo.freezeZ = false;
		}
		/*!
		 *@brief	デストラクタ。
		 */
		~CAnimationClip();
		/// <summary>
		/// アニメーションクリップを同期ロード。
		/// </summary>
		/// <param name="filePath"></param>
		void Load(const char* filePath);
		/// <summary>
		/// 非同期ロード。
		/// IsLoaded関数を使用して、同期をとるようにしてください。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadAsync(const char* filePath)
		{
			m_tkaFile.LoadAsync(filePath);
		}
		/// <summary>
		/// 読み込みが終わっているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsLoaded() const
		{
			return m_tkaFile.IsLoaded();
		}
		/// <summary>
		/// キーフレームとアニメーションイベントを構築する。
		/// </summary>
		/// <remarks>
		/// ロードが終わったときに呼び出してください。
		/// </remarks>
		void BuildKeyFramesAndAnimationEvents();
		/*!
		*@brief	ループする？
		*/
		bool IsLoop() const
		{
			return m_isLoop;
		}
		/*!
		*@brief	ループフラグを設定する。
		*/
		void SetLoopFlag(bool flag)
		{
			m_isLoop = flag;
		}
		/*!
		*@brief
		*/
		const vector<keyFramePtrList>& GetKeyFramePtrListArray() const
		{
			return m_keyFramePtrListArray;
		}
		const keyFramePtrList& GetTopBoneKeyFrameList() const
		{
			return *m_topBoneKeyFramList;
		}
		/*!
		 *@brief	クリップ名を取得。
		 */
		const wchar_t* GetName() const
		{
			return m_clipName.c_str();
		}
		/*!
		*@brief	アニメーションをフリーズするボーンの情報を設定。
		*@details
		* フリーズされたボーンのY方向の移動量はすべてのボーンから打ち消されます。
		*  →なにで使える？
		*		→アニメーションに移動量が入っているジャンプモーションとかで使える。
		*/
		void SetFreezeBoneInfo(int boneNo, bool freezeX, bool freezeY, bool freezeZ)
		{
			m_freezeBoneInfo.boneNo = boneNo;
			m_freezeBoneInfo.freezeX = freezeX;
			m_freezeBoneInfo.freezeY = freezeY;
			m_freezeBoneInfo.freezeZ = freezeZ;
		}
		/*!
		*@brief	X方向のアニメーションをフリーズしているボーンの番号を取得。
		*/
		const SFreezeBoneInfo& GetFreezeBoneInfo() const
		{
			return m_freezeBoneInfo;
		}
		/*!
		*@brief	アニメーションイベントを取得。
		*/
		std::unique_ptr<CAnimationEvent[]>& GetAnimationEvent()
		{
			return m_animationEvent;
		}

		/// <summary>
		/// アニメーションイベントの数を取得
		/// </summary>
		/// <returns></returns>
		int GetNumAnimationEvent() const
		{
			return m_tkaFile.GetNumAnimationEvent();
		}
	private:
		using KeyframePtr = unique_ptr<KeyFrame>;
		std::wstring m_clipName;	//!<アニメーションクリップの名前。
		bool m_isLoop = false;	//!<ループフラグ。
		vector<KeyframePtr>					m_keyframes;				//キーフレーム。
		vector<keyFramePtrList>				m_keyFramePtrListArray;		//ボーンごとのキーフレームのリストを管理するための配列。
		unique_ptr<CAnimationEvent[]>		m_animationEvent;			//アニメーションイベント。
		int									m_numAnimationEvent = 0;	//アニメーションイベントの数。
		keyFramePtrList*					m_topBoneKeyFramList = nullptr;
		SFreezeBoneInfo						m_freezeBoneInfo;	//!<フリーズさせるボーン情報。
		CTkaFile							m_tkaFile;			//tkaファイル
	};
	using CAnimationClipPtr = std::unique_ptr<CAnimationClip>;
}