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
		/// ディスクリプタヒープにConstantBufferViewを登録。
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistConstantBufferView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
    private:
		ComPtr<ID3D12Resource>	m_constantBuffer;	//定数バッファ。
		void* m_constBufferCPU = nullptr;			//CPU側からアクセスできるする定数バッファのアドレス。
		int m_size = 0;								//定数バッファのサイズ。
		int m_allocSize = 0;
    };
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12