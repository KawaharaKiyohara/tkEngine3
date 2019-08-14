#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

namespace tkEngine{
    class CConstantBufferDx12 : Noncopyable{
    public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CConstantBufferDx12();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="size">定数バッファのサイズ。</param>
		/// <param name="srcData">ソースデータ。nullを指定することも可能。</param>
		void Init(int size, void* srcData);

		/// <summary>
		/// 定数バッファの中身を更新。
		/// </summary>
		/// <param name="data"></param>
		void Update(void* data);
		/// <summary>
		/// ディスクリプタヒープの取得。
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12DescriptorHeap> GetDiscriptorHeap() const
		{
			return m_cbvHeap;
		}
    private:
		ComPtr<ID3D12Resource>	m_constantBuffer;	//定数バッファ。
		ComPtr<ID3D12DescriptorHeap> m_cbvHeap;		//ディスクリプタヒープ。
		void* m_constBufferCPU = nullptr;			//CPU側からアクセスできるする定数バッファのアドレス。
		int m_size = 0;								//定数バッファのサイズ。
    };
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12