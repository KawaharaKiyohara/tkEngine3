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
		/// 初期化処理。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">インデックスバッファのストライド。</param>
		void Init(int size, int stride)
		{
			m_count = size / stride;
			InitSub( size, stride );
		}
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
		/// <summary>
		/// サブクラスで定義する初期化関数。
		/// </summary>
		/// <remarks>
		/// グラフィックスAPIに依存する初期化処理はこの関数をオーバーライドして実装してください。
		/// </remarks>
		/// <param name="size"></param>
		/// <param name="stride"></param>
		virtual void InitSub(int size, int stride) = 0;
	private:
		int m_count = 0;		//インデックスの数。
	};
}