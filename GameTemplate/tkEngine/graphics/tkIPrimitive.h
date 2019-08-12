#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine{
	/// <summary>
	/// プリミティブのインターフェース。
	/// </summary>
	class IPrimitive : Noncopyable {
	public:
		/// <summary>
		/// プリミティブを作成。
		/// </summary>
		/// <param name="vertexBufferSize">頂点バッファのサイズ。</param>
		/// <param name="vertexBufferStride">頂点バッファのストライド。</param>
		/// <param name="indexBufferSize">インデックスバッファのサイズ。</param>
		/// <param name="indexBufferStride">インデックスバッファのストライド。</param>
		/// <param name="enPrimTopology">プリミティブのトポロジー。</param>
		/// <returns></returns>
		static std::unique_ptr<IPrimitive> Create(
			int vertexBufferSize,
			int vertexBufferStride,
			int indexBufferSize,
			int indexBufferStride,
			EnPrimitiveTopology enPrimTopology );
		/// <summary>
		/// 描画。
		/// </summary>
		virtual void Draw(CRenderContext& rc) = 0;
	protected:
		UPIVertexBuffer	m_vertexBuffer;	//頂点バッファ。
		UPIIndexBuffer m_indexBuffer;	//インデックスバッファ。
		EnPrimitiveTopology m_topology = enPrimitiveTopology_Undex;	//トポロジー。
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
	};
}