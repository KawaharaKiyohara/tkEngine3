#pragma once

#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/tkMeshParts.h"

namespace tkEngine {
	/// <summary>
	/// モデルクラス。
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// tkmファイルをロ―ド。
		/// </summary>
		/// <remarks>
		/// IsInited関数を利用して、同期をとるようにしてください。
		/// </remarks>
		/// <param name="filePath">tkmファイルのファイルパス。</param>
		void LoadTkmFileAsync(const char* filePath);
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
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mWorld">ワールド行列</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		void Draw(IRenderContext& rc, const CMatrix& mWorld, const CMatrix& mView, const CMatrix& mProj);
	private:
		CTkmFile m_tkmFile;			//tkmファイル。
		UPIMeshParts m_meshParts;	//メッシュパーツ。
	};
}