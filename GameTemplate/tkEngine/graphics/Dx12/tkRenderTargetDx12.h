#pragma once
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkTextureDx12.h"

namespace tkEngine {
	class CGraphicsEngineDx12;

	/// <summary>
	/// レンダリングターゲット。
	/// </summary>
	class CRenderTargetDx12 : Noncopyable {
	public:
		/// <summary>
		/// レンダリングターゲットの作成。
		/// </summary>
		/// <param name="w">レンダリングターゲットの幅</param>
		/// <param name="h">レンダリングターゲットの高さ</param>
		/// <param name="mipLevel">ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。</param>
		/// <param name="arraySize">テクスチャ配列のサイズ</param>
		/// <param name="colorFormat">カラーバッファのフォーマット。</param>
		/// <param name="depthStencilFormat">深度ステンシルバッファのフォーマット。</param>
		/// <returns>trueが返ってきたら作成成功</returns>
		bool Create(
			int w,
			int h,
			int mipLevel,
			int arraySize,
			DXGI_FORMAT colorFormat,
			DXGI_FORMAT depthStencilFormat
		);
		/// <summary>
		/// CPU側のレンダリングターゲットのディスクリプタハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
		{
			return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		/// <summary>
		/// CPU側のデプスステンシルバッファのディスクリプタハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
		{
			return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		/// <summary>
		/// レンダリングターゲットとなるテクスチャを取得。
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetRenderTargetTexture()
		{
			return m_renderTargetTexture;
		}
		/// <summary>
		/// デプスステンシルバッファが存在しているか判定
		/// </summary>
		/// <returns></returns>
		bool IsExsitDepthStencilBuffer() const
		{
			return m_depthStencilTexture;
		}
		/// <summary>
		/// レンダリングターゲットの幅を取得。
		/// </summary>
		/// <returns></returns>
		int GetWidth() const
		{
			return m_width;
		}
		/// <summary>
		/// レンダリングターゲットの高さを取得。
		/// </summary>
		/// <returns></returns>
		int GetHeight() const
		{
			return m_height;
		}
	private:
		/// <summary>
		/// ディスクリプタヒープを作成。
		/// </summary>
		/// <param name="ge">グラフィックエンジン</param>
		/// <param name="d3dDevice">D3Dデバイス</param>
		/// <returns></returns>
		bool CreateDescriptorHeap(CGraphicsEngineDx12& ge, ComPtr<ID3D12Device>& d3dDevice);
		/// <summary>
		/// レンダリングターゲットとなるテクスチャを作成。
		/// </summary>
		/// <param name="ge">グラフィックエンジン</param>
		/// <param name="d3dDevice">D3Dデバイス</param>
		/// <param name="w">テクスチャの幅</param>
		/// <param name="h">テクスチャの高さ</param>
		/// <param name="mipLevel">ミップマップレベル</param>
		/// <param name="arraySize">テクスチャ配列のサイズ</param>
		/// <param name="format">テクスチャのフォーマット</param>
		/// <returns>trueが返ってきたら成功。</returns>
		bool CreateRenderTargetTexture(
			CGraphicsEngineDx12& ge, 
			ComPtr<ID3D12Device>& d3dDevice,
			int w, 
			int h,
			int mipLevel,
			int arraySize,
			DXGI_FORMAT format
		);
		/// <summary>
		/// 深度ステンシルバッファとなるテクスチャを作成。
		/// </summary>
		/// <param name="ge">グラフィックエンジン</param>
		/// <param name="d3dDevice">D3Dデバイス</param>
		/// <param name="w">テクスチャの幅</param>
		/// <param name="h">テクスチャの高さ</param>
		/// <param name="format">テクスチャのフォーマット</param>
		/// <returns>trueが返ってきたら成功</returns>
		bool CreateDepthStencilTexture(
			CGraphicsEngineDx12& ge,
			ComPtr<ID3D12Device>& d3dDevice,
			int w,
			int h,
			DXGI_FORMAT format);
		/// <summary>
		/// ディスクリプタの作成。
		/// </summary>
		/// <param name="d3dDevice">D3Dデバイス</param>
		/// <returns>trueが返ってｋチアら成功。</returns>
		void CreateDescriptor(ComPtr<ID3D12Device>& d3dDevice);
	private:
		CTextureDx12 m_renderTargetTexture;
		ComPtr<ID3D12Resource> m_renderTargetTextureDx12;	//レンダリングターゲットとなるテクスチャ。
		ComPtr< ID3D12Resource> m_depthStencilTexture;		//深度ステンシルバッファとなるテクスチャ。
		ComPtr< ID3D12DescriptorHeap>		m_rtvHeap;		//RTV用のディスクリプタヒープ。
		ComPtr< ID3D12DescriptorHeap>		m_dsvHeap;		//深度ステンシルバッファビューのディスクリプタヒープ。
		UINT m_rtvDescriptorSize = 0;						//フレームバッファのディスクリプタのサイズ。
		UINT m_dsvDescriptorSize = 0;						//深度ステンシルバッファのディスクリプタのサイズ。
		int m_width = 0;									//レンダリングターゲットの幅。
		int m_height = 0;									//レンダリングターゲットの高さ。
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
