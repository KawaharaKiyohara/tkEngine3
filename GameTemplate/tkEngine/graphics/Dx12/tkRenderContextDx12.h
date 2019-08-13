#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"

namespace tkEngine {
	/// <summary>
	/// レンダリングコンテキスト。
	/// </summary>
	class CRenderContextDx12 : public IRenderContext {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="commandList">コマンドリスト。</param>
		void Init(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
		/// <summary>
		/// コマンドリストを取得。
		/// </summary>
		/// <returns>コマンドリスト。</returns>
		ComPtr<ID3D12GraphicsCommandList> GetCommandList()
		{
			return m_commandList;
		}
		/// <summary>
		/// コマンドリストを設定。
		/// </summary>
		/// <param name="commandList">コマンドリスト。</param>
		void SetCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
	private:
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//コマンドリスト。
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

