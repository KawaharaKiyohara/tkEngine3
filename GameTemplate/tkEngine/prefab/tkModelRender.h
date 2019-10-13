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
			/// <remarks>
			/// templateの黒魔術を使って、配列のサイズを調べられるようにしてます。
			/// 危険、マネするな。
			/// </remarks>
			/// <param name="tkmFilePath">tkmファイルのパス</param>
			/// <param name="tkaFilePaths">tkaファイルのパスの配列</param>
			template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
			void Init(const char* tkmFilePath, TKA_FILE_ARRAY (&tkaFilePaths)[NUM_TKA_FILE])
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFileAsync(tkmFilePath);
				for (auto i = 0; i < NUM_TKA_FILE; i++) {
					m_tkaFilePaths.push_back(tkaFilePaths[i]);
				}
				//初期化ステータスをモデル初期化待ちにする。
				m_initStatus = enInitStatus_WaitInitModel;
			}
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
				enInitStatus_NotCallInitFunc,			//初期化関数が呼ばれていない。
				enInitStatus_WaitInitModel,				//モデル初期化待ち。
				enInitStatus_WaitInitSkeleton,			//スケルトンの初期化待ち。
				enInitStatus_WaitInitAnimationClips,	//アニメーションクリップの初期化待ち。
				enInitStatus_Completed,					//初期化完了。
			};
			using CUPAnimationClip = unique_ptr<CAnimationClip>;
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//初期化ステータス。
			CModel m_model;		//モデル。
			CVector3 m_position = g_vec3Zero;			//座標。
			CQuaternion	m_rotation = g_quatIdentity;	//回転。
			CVector3 m_scale = g_vec3One;				//拡大率。
			CSkeleton m_skeleton;						//スケルトン。
			string m_tkmFilePath;						//tkmファイルのファイルパス。
			vector< string > m_tkaFilePaths;			//tkaファイルのファイルパスのリスト。
			vector< CUPAnimationClip> m_animationClips;	//アニメーションクリップの配列。
		};
	}
}