#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// 頂点バッファのインターフェース。
	/// </summary>
	class CVertexBufferDx12 : public IVertexBuffer {
		
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="size">頂点バッファのサイズ。</param>
		/// <param name="stride">頂点バッファのストライド。一個の頂点のサイズ。</param>
		CVertexBufferDx12(int size, int stride);
		/// <summary>
		/// 頂点データを頂点バッファにコピー。
		/// </summary>
		/// <param name="srcVertices">コピー元の頂点データ。</param>
		void Copy(void* srcVertices) override final;
		/// <summary>
		/// 頂点バッファビューを取得。
		/// </summary>
		/// <returns>ビュー</returns>
		D3D12_VERTEX_BUFFER_VIEW& GetView()
		{
			return m_vertexBufferView;
		}
	private:
		ComPtr<ID3D12Resource> m_vertexBuffer;			//頂点バッファ。
		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;	//頂点バッファビュー。
	};
}