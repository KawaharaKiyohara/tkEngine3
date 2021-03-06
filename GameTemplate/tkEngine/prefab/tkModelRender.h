/// <summary>
/// モデルレンダラー。
/// </summary>
#pragma once

#include "tkEngine/graphics/tkModel.h"
#include "tkEngine/graphics/tkAnimation.h"

namespace tkEngine {
	namespace prefab {
		/// <summary>
		/// 3Dモデルを表示する機能を提供するクラス。
		/// </summary>
		class CModelRender : public IGameObject {
		public:
			/// <summary>
			/// 同期の初期化処理。
			/// </summary>
			/// <remarks>
			/// templateの黒魔術を使って、配列のサイズを調べられるようにしてます。
			/// 危険、マネするな。
			/// </remarks>
			/// <param name="tkmFilePath">tkmファイルのパス</param>
			/// <param name="tkaFilePaths">tkaファイルのパスの配列</param>
			template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
			void Init(const char* tkmFilePath, TKA_FILE_ARRAY(&tkaFilePaths)[NUM_TKA_FILE])
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFile(tkmFilePath);
				for (auto i = 0; i < NUM_TKA_FILE; i++) {
					m_tkaFilePaths.push_back(tkaFilePaths[i]);
				}
				
				//@todo InitCommon(tkaFilePaths);
			}
			void Init(const char* tkmFilePath)
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFile(tkmFilePath);
			//@todo	InitCommon(tkaFilePaths);
			}
			/// <summary>
			/// 非同期の初期化処理。
			/// </summary>
			/// <remarks>
			/// templateの黒魔術を使って、配列のサイズを調べられるようにしてます。
			/// 危険、マネするな。
			/// </remarks>
			/// <param name="tkmFilePath">tkmファイルのパス</param>
			/// <param name="tkaFilePaths">tkaファイルのパスの配列</param>
			template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
			void InitAsync(const char* tkmFilePath, TKA_FILE_ARRAY(&tkaFilePaths)[NUM_TKA_FILE])
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFileAsync(tkmFilePath);
				for (auto i = 0; i < NUM_TKA_FILE; i++) {
					m_tkaFilePaths.push_back(tkaFilePaths[i]);
				}
				//初期化ステータスをモデル初期化待ちにする。
				m_initStatus = enInitStatus_WaitInitModel;
			}
			void InitAsync(const char* tkmFilePath)
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFileAsync(tkmFilePath);
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
			/// 座標を取得。
			/// </summary>
			CVector3 GetPosition() const
			{
				return m_position;
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
			void SetPosition(float x, float y, float z)
			{
				m_position.Set(x, y, z);
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
			void SetScale(float xScale, float yScale, float zScale)
			{
				m_scale.Set(xScale, yScale, zScale);
			}
			void SetAnimationLoopFlag(int animNo, bool flag)
			{
				m_animationClips.at(animNo)->SetLoopFlag(flag);
			}
			void PlayAnimation(int animNo, float interpolateTime)
			{
				m_animation.Play(animNo, interpolateTime);
			}
			/// <summary>
			/// シャドウマップへのレンダリング。
			/// </summary>
			/// <param name="renderContext">レンダリングコンテキスト</param>
			/// <param name="mLightView">ライトビュー行列</param>
			/// <param name="mLightProj">ライトプロジェクション行列。</param>
			void RenderToShadowMap(IRenderContext& renderContext, const CMatrix& mLightView, const CMatrix& mLightProj) override;
			/// <summary>
			/// シャドウキャスターフラグを設定。
			/// </summary>
			/// <remarks>
			/// シャドウキャスターフラグがtrueになっていると、
			/// シャドウマップへのレンダリングが行われます。
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowCasterFlag(bool flag)
			{
				m_isShadowCaster = flag;
			}
			/// <summary>
			/// シャドウレシーバーフラグを設定。
			/// </summary>
			/// <remarks>
			/// シャドウレシーバーフラグがtrueになっていると、
			/// シャドウマップを参照して、他のオブジェクトからの影が落とされます。
			/// </remarks>
			/// <param name="flag"></param>
			void SetShadowReceiverFlag(bool flag)
			{
				m_isShadowReceiver = flag;
			}
			/// <summary>
			/// footstepボーンの変化量を取得。
			/// </summary>
			/// <returns>footstepボーンの変化量</returns>
			CVector3 GetFootstepBoneDeltaValue() const
			{
				return m_animation.CalcFootstepDeltaValueInWorldSpace(m_rotation, m_scale);
			}
			/// <summary>
			/// モデルを取得。
			/// </summary>
			/// <returns></returns>
			CModel& GetModel()
			{
				return m_model;
			}
		private:
			/// <summary>
			/// 同期初期化処理の共通処理。
			/// </summary>
			/// <param name="tkaFile">tkaファイル</param>
			void InitCommon(CTkaFile* tkaFile);
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
			CAnimation m_animation;
			bool m_isShadowCaster = false;				//シャドウキャスタ―フラグ。
			bool m_isShadowReceiver = false;			//シャドウレシーバーフラグ。
		};
	}
}