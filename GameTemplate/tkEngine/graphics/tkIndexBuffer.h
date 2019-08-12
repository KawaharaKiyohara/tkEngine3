#pragma once

namespace tkEngine {
	class IModelImp;
	/// <summary>
	/// インデックスバッファのインターフェースクラス。
	/// </summary>
	class IIndexBuffer : Noncopyable {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		virtual ~IIndexBuffer() {}
		/// <summary>
		/// インデックスバッファを作成。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ</param>
		/// <returns>作成されたインデックスバッファ。</returns>
		static std::unique_ptr<IIndexBuffer> Create( int size, int stride );
		/// <summary>
		/// 頂点データを頂点バッファにコピー。
		/// </summary>
		/// <param name="srcVertices">コピー元の頂点データ。</param>
		virtual void Copy(void* srcIndecies) = 0;
	private:
	};
}