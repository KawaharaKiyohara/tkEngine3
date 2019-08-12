#pragma once

#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// インデックスバッファ。
	/// </summary>
	class CIndexBuffer : public IIndexBuffer {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド。</param>
		CIndexBuffer(int size, int stride);
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		virtual ~CIndexBuffer() {}
		/// <summary>
		/// インデックスデータをインデックスバッファにコピー。
		/// </summary>
		/// <param name="srcIndecies">コピー元のインデックスデータ。</param>
		void Copy(void* srcIndecies) override final;
	private:
		ComPtr<ID3D12Resource> m_indexBuffer;			//インデックスバッファ。
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;	//インデックスバッファビュー。
	};
}