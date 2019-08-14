#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"
#include "tkEngine/graphics/tkShape.h"

namespace tkEngine {
	/// <summary>
	/// 三角形の形状クラス。
	/// </summary>
	class CTriangleShapeDx12 : public IShape
	{
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CTriangleShapeDx12();
	
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列。</param>
		void Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// 定数バッファ。
		/// </summary>
		/// <remarks>
		/// この構造体を変更したら、Primitive.fxのCBも変更するように。
		/// </remarks>
		struct SConstantBuffer {
			CMatrix mWorld;		//ワールド行列。
			CMatrix mView;		//ビュー行列。
			CMatrix mProj;		//プロジェクション行列。
		};
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
		CShaderDx12 m_vs;								//頂点シェーダー。
		CShaderDx12 m_ps;								//ピクセルシェーダー。
		CConstantBufferDx12 m_constantBuffer;			//定数バッファ。
	};

}

#endif