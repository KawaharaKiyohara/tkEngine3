/// <summary>
/// モデルレンダラー
/// </summary>

#include "tkEngine/tkEnginePrecompile.h"
#include "tkEngine/prefab/tkModelRender.h"
#include<fstream>
#include<iostream>

namespace tkEngine {
	namespace prefab {
		bool CModelRender::Start()
		{
			bool inited = false;
			switch (m_initStatus) {
			case enInitStatus_NotCallInitFunc:
				TK_LOG("Please call Init func");
				break;
			case enInitStatus_WaitInitModel:
				if (m_model.IsInited()) {
					//モデルの初期化完了したのでメッシュパーツを構築する。
					m_model.CreateMeshParts();
					//続いてスケルトン。
					//スケルトンのデータを読み込み。
					std::string skeletonFilePath = m_tkmFilePath;
					int pos = (int)skeletonFilePath.find(".tkm");
					skeletonFilePath.replace(pos, 4, ".tks");
					std::ifstream ifs(skeletonFilePath);
					if (ifs.is_open()) {
						//tksファイルがあればtksファイルを読み込む。
						m_skeleton.LoadAsync(skeletonFilePath.c_str());
						m_initStatus = enInitStatus_WaitInitSkeleton;
					}
					else {
						//ない。
						m_initStatus = enInitStatus_Completed;
					}
				}
				break;
			case enInitStatus_WaitInitSkeleton:
				if (m_skeleton.IsLoaded()) {
					m_skeleton.BuildBoneMatrices();
					//スケルトンとモデルを関連付ける。
					m_model.BindSkeleton(m_skeleton);
					//続いてアニメーションクリップ
					if (m_tkaFilePaths.empty() == false) {
						for (auto& tkaFilePath : m_tkaFilePaths) {
							auto animClip = make_unique<CAnimationClip>();
							animClip->LoadAsync(tkaFilePath.c_str());
							m_animationClips.push_back(move(animClip));
						}
						m_initStatus = enInitStatus_WaitInitAnimationClips;
					}
					else {
						m_initStatus = enInitStatus_Completed;
					}
				}
				break;
			case enInitStatus_WaitInitAnimationClips: {
				bool isLoaded = true;
				for (auto& animClip : m_animationClips) {
					if (animClip->IsLoaded() == false) {
						isLoaded = false;
						break;
					}
				}
				if (isLoaded == true) {
					//全部のアニメーションクリップのロードが完了した。
					//ロードが完了したので、キーフレームとアニメーションイベントの構築を行う。
					for (auto& animClip : m_animationClips) {
						//@todo カリカリ。
						animClip->SetLoopFlag(true);
						animClip->BuildKeyFramesAndAnimationEvents();
					}
					//アニメーションを初期化。
					m_animation.Init(m_skeleton, m_animationClips);
					m_initStatus = enInitStatus_Completed;
				}
			}break;
			case enInitStatus_Completed:
				//初期化完了。
				inited = true;
				break;
			}
			return inited;
		}
		void CModelRender::Update()
		{
			//モデルを更新。
			m_model.Update(m_position, m_rotation, m_scale);
			if (m_animation.IsInited()) {
				//アニメーションを再生。
				m_animation.Update(g_gameTime->GetFrameDeltaTime());
			}
			if (m_skeleton.IsInited()) {
				//スケルトンを更新。
				m_skeleton.Update(m_model.GetWorldMatrix());
			}
		}
		void CModelRender::ForwardRender(IRenderContext& renderContext)
		{
			m_model.Draw(renderContext, g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());
		}
		void CModelRender::RenderToShadowMap(IRenderContext& renderContext, const CMatrix& mLightView, const CMatrix& mLightProj)
		{
			if (m_isShadowCaster) {
				m_model.Draw(renderContext, mLightView, mLightProj );
			}
		}
	}
}