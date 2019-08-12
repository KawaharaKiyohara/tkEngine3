#pragma once

namespace tkEngine {
	/// <summary>
	/// 頂点バッファのインターフェース。
	/// </summary>
	class IVertexBuffer : Noncopyable {
	public:
		virtual~ IVertexBuffer() {}
		/// <summary>
		/// 頂点バッファの作成。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ</param>
		/// <param name="stride">頂点のサイズ。</param>
		/// <returns></returns>
		static std::unique_ptr<IVertexBuffer> Create(int size, int stride);
		/// <summary>
		/// 頂点データを頂点バッファにコピー。
		/// </summary>
		/// <param name="srcVertices">コピー元の頂点データ。</param>
		virtual void Copy(void* srcVertices) = 0;
	private:
	};
}