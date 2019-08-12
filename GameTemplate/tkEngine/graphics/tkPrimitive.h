#pragma once

#include "tkEngine/graphics/tkVertexBuffer.h"
#include "tkEngine/graphics/tkIndexBuffer.h"

namespace tkEngine{
	/// <summary>
	/// プリミティブクラス。
	/// </summary>
	class CPrimitive : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="srcVertexData">頂点バッファのソースデータ。</param>
		/// <param name="vertexBufferSize">頂点バッファのサイズ。</param>
		/// <param name="vertexBufferStride">頂点バッファのストライド。</param>
		/// <param name="srcIndexData">インデックスバッファのソースデータ。</param>
		/// <param name="indexBufferSize">インデックスバッファのサイズ。</param>
		/// <param name="indexBufferStride">インデックスバッファのストライド。</param>
		/// <param name="enPrimTopology">プリミティブのトポロジー。</param>
		/// <returns></returns>
		void Init(
			void* srcVertexData,
			int vertexBufferSize,
			int vertexBufferStride,
			void* srcIndexData,
			int indexBufferSize,
			int indexBufferStride,
			EnPrimitiveTopology enPrimTopology);
		/// <summary>
		/// 頂点バッファを取得。
		/// </summary>
		/// <returns></returns>
		UPIVertexBuffer& GetVertexBuffer() 
		{
			return m_vertexBuffer;
		}
		/// <summary>
		/// インデックスバッファを取得。
		/// </summary>
		/// <returns></returns>
		UPIIndexBuffer& GetIndexBuffer()
		{
			return m_indexBuffer;
		}
		/// <summary>
		/// プリミティブのトポロジーを取得。
		/// </summary>
		/// <returns></returns>
		EnPrimitiveTopology GetPrimitiveTopology() const
		{
			return m_topology;
		}
	protected:
		UPIVertexBuffer	m_vertexBuffer;	//頂点バッファ。
		UPIIndexBuffer m_indexBuffer;	//インデックスバッファ。
		EnPrimitiveTopology m_topology = enPrimitiveTopology_Undex;	//トポロジー。
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
	};
}