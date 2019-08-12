#pragma once

namespace tkEngine {
	class IModelImp;
	class IIndexBuffer;
	/// <summary>
	/// IIndexBufferのユニークポインタ型の別名定義。
	/// </summary>
	using UPIIndexBuffer = std	::unique_ptr<IIndexBuffer>;

	/// <summary>
	/// インデックスバッファのインターフェースクラス。
	/// </summary>
	class IIndexBuffer : public Noncopyable{
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
		static UPIIndexBuffer Create( int size, int stride );
		/// <summary>
		/// 頂点データを頂点バッファにコピー。
		/// </summary>
		/// <param name="srcVertices">コピー元の頂点データ。</param>
		virtual void Copy(void* srcIndecies) = 0;
		/// <summary>
		/// 型変換。
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
		/// <summary>
		/// インデックスの数を取得。
		/// </summary>
		/// <returns>インデックスの数。</returns>
		int GetCount() const
		{
			return m_count;
		}
	private:
		int m_count = 0;		//インデックスの数。
	};
}