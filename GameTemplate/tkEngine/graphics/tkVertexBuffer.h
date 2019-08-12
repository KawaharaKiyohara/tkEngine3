#pragma once

namespace tkEngine {
	class IVertexBuffer;
	//ユニークポインタの別名定義。クラスの先頭にUPをつける命名規則。
	using UPIVertexBuffer = std::unique_ptr < IVertexBuffer >;
	/// <summary>
	/// 頂点バッファのインターフェース。
	/// </summary>
	class IVertexBuffer : public Noncopyable{
	public:
		virtual~ IVertexBuffer() {}
		/// <summary>
		/// 頂点バッファの作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">頂点のサイズ。</param>
		/// <returns></returns>
		static UPIVertexBuffer Create(int size, int stride);
		/// <summary>
		/// 頂点データを頂点バッファにコピー。
		/// </summary>
		/// <param name="srcVertices">コピー元の頂点データ。</param>
		virtual void Copy(void* srcVertices) = 0;
		/// <summary>
		/// 型変換。
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
	private:
	};
	
}