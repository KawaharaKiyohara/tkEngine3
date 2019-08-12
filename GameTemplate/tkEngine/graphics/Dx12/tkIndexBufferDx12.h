#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// インデックスバッファ。
	/// </summary>
	class CIndexBufferDx12 : public IIndexBuffer {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		/// <param name="size">インデックスバッファのサイズ。</param>
		/// <param name="stride">ストライド。</param>
		CIndexBufferDx12(int size, int stride);
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		virtual ~CIndexBufferDx12() {}
		/// <summary>
		/// インデックスデータをインデックスバッファにコピー。
		/// </summary>
		/// <param name="srcIndecies">コピー元のインデックスデータ。</param>
		void Copy(void* srcIndecies) override final;
		/// <summary>
		/// インデックスバッファビューを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_INDEX_BUFFER_VIEW& GetView()
		{
			return m_indexBufferView;
		}
	private:
		ComPtr<ID3D12Resource> m_indexBuffer;			//インデックスバッファ。
		D3D12_INDEX_BUFFER_VIEW m_indexBufferView;	//インデックスバッファビュー。
	};
}

#endif // #if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12