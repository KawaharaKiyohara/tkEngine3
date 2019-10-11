/// <summary>
/// モデルレンダラー。
/// </summary>
#pragma once

#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	namespace prefab {
		/// <summary>
		/// 3Dモデルを表示する機能を提供するクラス。
		/// </summary>
		class CModelRender : public IGameObject {
		public:
			/// <summary>
			/// 初期化。
			/// </summary>
			/// <param name="filePath"></param>
			void Init(const char* filePath);
			/// <summary>
			/// 開始処理
			/// </summary>
			/// <returns></returns>
			bool Start() override;
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update() override;
			/// <summary>
			/// フォワードレンダリングのパスで呼ばれる処理。
			/// </summary>
			/// <param name="renderContext"></param>
			void ForwardRender(IRenderContext& renderContext) override;
			/// <summary>
			/// モデルを移動させる。
			/// </summary>
			/// <param name="move">移動量</param>
			void Move(CVector3 move)
			{
				m_position += move;
			}
			/// <summary>
			/// モデルを回転させる。
			/// </summary>
			/// <param name="rot">回転量</param>
			void Rotate(CQuaternion rot)
			{
				//回転を加算する。
				m_rotation *= rot;
			}
			/// <summary>
			/// 座標を直接設定。
			/// </summary>
			/// <param name="pos">座標</param>
			void SetPosition(CVector3 pos)
			{
				m_position = pos;
			}
			/// <summary>
			/// 回転を直接設定。
			/// </summary>
			/// <param name="rot"></param>
			void SetRotation(CQuaternion rot)
			{
				m_rotation = rot;
			}
			/// <summary>
			/// 拡大率を直接設定。
			/// </summary>
			/// <param name="scale"></param>
			void SetScale(CVector3 scale)
			{
				m_scale = scale;
			}
		private:
			/// <summary>
			/// 初期化ステータス。
			/// </summary>
			enum EnInitStatus {
				enInitStatus_NotCallInitFunc,	//初期化関数が呼ばれていない。
				enInitStatus_WaitInitModel,		//モデル初期化待ち。
				enInitStatus_Completed,			//初期化完了。
			};
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//初期化ステータス。
			CModel m_model;		//モデル。
			CVector3 m_position = CVector3::Zero;			//!<座標。
			CQuaternion	m_rotation = CQuaternion::Identity;	//!<回転。
			CVector3 m_scale = CVector3::One;				//!<拡大率。
			CSkeleton m_skeleton;							//!<スケルトン。
		};
	}
}