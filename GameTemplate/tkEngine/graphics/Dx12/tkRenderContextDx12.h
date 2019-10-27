#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"

namespace tkEngine {
	class CConstantBufferDx12;
	class CTextureDx12;
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
		/// コマンドリストを設定。
		/// </summary>
		/// <param name="commandList">コマンドリスト。</param>
		void SetCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
		/// <summary>
		/// プリミティブのトポロジーを設定。
		/// </summary>
		/// <remarks>
		/// ID3D12GraphicsCommandList::のIASetPrimitiveTopologyのラッパー関数。
		/// 詳細はMicrosoftのヘルプを参照。
		/// </remarks>
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
		{
			m_commandList->IASetPrimitiveTopology(topology);
		}
		void SetPrimitiveTopology(EnPrimitiveTopology topology)
		{
			m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
		}
		/// <summary>
		/// 頂点バッファを設定。
		/// </summary>
		void SetVertexBuffer(CVertexBufferDx12& vb)
		{
			m_commandList->IASetVertexBuffers(0, 1, &vb.GetView());
		}
		void SetVertexBuffer(UPIVertexBuffer& vb)
		{
			auto vbDx12 = vb->As<CVertexBufferDx12>();
			m_commandList->IASetVertexBuffers(0, 1, &vbDx12->GetView());
		}
		/// <summary>
		/// インデックスバッファを設定。
		/// </summary>
		/// <param name="ib"></param>
		void SetIndexBuffer(CIndexBufferDx12& ib)
		{
			m_commandList->IASetIndexBuffer(&ib.GetView());
		}
		void SetIndexBuffer(UPIIndexBuffer& ib)
		{
			auto ibDx12 = ib->As<CIndexBufferDx12>();
			m_commandList->IASetIndexBuffer(&ibDx12->GetView());
		}
		void SetIndexBuffer(UPCIndexBufferDx12& ib)
		{
			m_commandList->IASetIndexBuffer(&ib->GetView());
		}
	
		/// <summary>
		/// 定数バッファ、シェーダーリソース、UAV(UnorderResrouceView)をディスクリプタヒープに登録する。
		/// </summary>
		/// <param name="descriptorHeap">ディスクリプタヒープ</param>
		/// <param name="numCBR">定数バッファの数</param>
		/// <param name="constantBufferArray">定数バッファの配列</param>
		/// <param name="numSRV">シェーダーリソースの数</param>
		/// <param name="srvArray">シェーダーリソースの配列</param>
		void SetCBR_SRV_UAV(
			ID3D12DescriptorHeap* descriptorHeap,
			int numConstantBuffer,
			CConstantBufferDx12* constantBufferArray[],
			int numShaderResource,
			IShaderResourceDx12* shaderResourceArray[]
		);

		/// <summary>
		/// インデックスつきプリミティブを描画。
		/// </summary>
		/// <param name="indexCount">インデックスの数。</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}
		/// <summary>
		/// ルートシグネチャを設定。
		/// </summary>
		void SetRootSignature(ComPtr<ID3D12RootSignature>& rootSignature)
		{
			m_commandList->SetGraphicsRootSignature(rootSignature.Get());
		}
		void SetRootSignature(CRootSignatureDx12& rootSignature)
		{
			m_commandList->SetGraphicsRootSignature(rootSignature.Get());
		}
		/// <summary>
		/// パイプラインステートを設定。
		/// </summary>
		void SetPipelineState(ComPtr<ID3D12PipelineState>& pipelineState)
		{
			m_commandList->SetPipelineState(pipelineState.Get());
		}
		/// <summary>
		/// ディスクリプタヒープを設定。
		/// </summary>
		void SetDescriptorHeap(ComPtr< ID3D12DescriptorHeap>& descHeap)
		{
			m_descriptorHeaps[0] = descHeap.Get();	//カリカリカリ
			m_commandList->SetDescriptorHeaps(1, m_descriptorHeaps);
		}
		void SetGraphicsRootDescriptorTable(
			UINT RootParameterIndex,
			D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
		{
			m_commandList->SetGraphicsRootDescriptorTable(
				RootParameterIndex,
				BaseDescriptor
			);
		}
	private:
		enum { MAX_DESCRIPTOR_HEAP = 4 };	//ディスクリプタヒープの最大数。
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//コマンドリスト。
		ID3D12DescriptorHeap* m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

