#pragma once

#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/tkMeshParts.h"
#include "tkEngine/graphics/tkSkeleton.h"

namespace tkEngine {
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// tkmファイルを非同期ロ―ド。
		/// </summary>
		/// <remarks>
		/// IsInited関数を利用して、同期をとるようにしてください。
		/// </remarks>
		/// <param name="filePath">tkmファイルのファイルパス。</param>
		void LoadTkmFileAsync(const char* filePath);
		/// <summary>
		/// tkmファイルをロード。
		/// </summary>
		/// <param name="filePath">tkmファイルのファイルパス。</param>
		void LoadTkmFile(const char* filePath);
		/// <summary>
		/// メッシュパーツを作成する。
		/// </summary>
		/// <remarks>
		/// この関数はLoadTkmFileAsync関数などを利用して、
		/// tkmファイルをロードした後で呼び出してください。
		/// </remarks>
		void CreateMeshParts();
		/// <summary>
		/// 初期化が終わっているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsInited() const;
		/// <summary>
		/// 更新。
		/// 必ず毎フレーム実行してください。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void Update(CVector3 pos, CQuaternion rot, CVector3 scale);
		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		void BindSkeleton(CSkeleton& skeleton)
		{
			m_meshParts->BindSkeleton(skeleton);
		}
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		void Draw(IRenderContext& rc, CMatrix mView, CMatrix mProj);
		/// <summary>
		/// ワールド行列を取得。
		/// </summary>
		/// <returns></returns>
		const CMatrix& GetWorldMatrix() const
		{
			return m_world;
		}
	private:
		CMatrix m_world;			//ワールド行列。
		CTkmFile m_tkmFile;			//tkmファイル。
		CSkeleton m_skeleton;		//スケルトン。
		UPIMeshParts m_meshParts;	//メッシュパーツ。
	};
}