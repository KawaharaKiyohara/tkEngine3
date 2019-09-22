/// <summary>
/// メッシュパーツクラス。
/// </summary>

#pragma once

namespace tkEngine {
	class CTkmFile;
	/// <summary>
	/// メッシュパーツのインターフェース。
	/// </summary>
	class IMeshParts : public Noncopyable {
	public:

		/// <summary>
		/// tkmファイルから初期化
		/// </summary>
		/// <param name="tkmFile">tkmファイル。</param>
		virtual void InitFromTkmFile(const CTkmFile& tkmFile) = 0;
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mWorld">ワールド行列</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mWorld, const CMatrix& mView, const CMatrix& mProj) = 0;
		/// <summary>
		/// 派生クラスに型変換。
		/// 
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
	};
}