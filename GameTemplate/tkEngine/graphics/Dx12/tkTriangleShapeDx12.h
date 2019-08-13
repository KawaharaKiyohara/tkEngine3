#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"

namespace tkEngine {
	/// <summary>
	/// 三角形の形状クラス。
	/// </summary>
	class CTriangleShapeDx12
	{
	public:

		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void Draw(IRenderContext& rc);
	private:
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
		CShaderDx12 m_vs;								//頂点シェーダー。
		CShaderDx12 m_ps;								//ピクセルシェーダー。
	};

	using CTriangleShape = CTriangleShapeDx12;
}

#endif