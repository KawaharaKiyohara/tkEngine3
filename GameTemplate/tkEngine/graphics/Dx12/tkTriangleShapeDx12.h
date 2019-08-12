#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/Dx12/tkPrimitiveDx12.h"

namespace tkEngine {
	/// <summary>
	/// 三角形の形状クラス。
	/// </summary>
	class CTriangleShapeDx12
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CTriangleShapeDx12();

	private:
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
	};

	using CTriangleShape = CTriangleShapeDx12;
}

#endif