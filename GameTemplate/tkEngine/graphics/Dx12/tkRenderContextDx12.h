#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"
#include "tkEngine/graphics/Dx12/tkDescriptorHeapDx12.h"
#include "tkEngine/graphics/Dx12/tkPipelineStateDx12.h"

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
		/// ビューポートを設定
		/// </summary>
		/// <param name="viewport">ビューポート</param>
		void SetViewport(D3D12_VIEWPORT& viewport)
		{
			m_commandList->RSSetViewports(1, &viewport);
		}
		/// <summary>
		/// シザリング矩形を設定
		/// </summary>
		/// <param name="rect"></param>
		void SetScissorRect(D3D12_RECT& rect)
		{
			m_commandList->RSSetScissorRects(1, &rect);
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
		/// <param name="constantBufferArray">定数バッファの配列</param>
		/// <param name="numSRV">シェーダーリソースの数</param>
		/// /// <param name="numCBR">定数バッファの数</param>
		/// <param name="srvArray">シェーダーリソースの配列</param>
		void SetCBR_SRV_UAV(
			CConstantBufferDx12* constantBufferArray[],
			IShaderResourceDx12* shaderResourceArray[],
			int numConstantBuffer,
			int numShaderResource
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
		void SetPipelineState(CPipelineStateDx12& pipelineState)
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
		/// <summary>
		/// レンダリングターゲットとビューポートを同時に設定する。
		/// </summary>
		/// <param name="renderTarget"></param>
		void SetRenderTargetAndViewport(CRenderTargetDx12& renderTarget)
		{
			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = static_cast<float>(renderTarget.GetWidth());
			viewport.Height = static_cast<float>(renderTarget.GetHeight());
			viewport.MinDepth = D3D12_MIN_DEPTH;
			viewport.MaxDepth = D3D12_MAX_DEPTH;
			SetViewport(viewport);
			SetRenderTarget(renderTarget);
		}
		/// <summary>
		/// レンダリングターゲットを設定
		/// </summary>
		void SetRenderTarget(CRenderTargetDx12& renderTarget)
		{
			auto rtvHandle = renderTarget.GetRTVCpuDescriptorHandle();
			if (renderTarget.IsExsitDepthStencilBuffer() ){
				auto dsvHandle = renderTarget.GetDSVCpuDescriptorHandle();
				m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
			}
			else {
				//デプスステンシルバッファはない。
				m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
			}
		}
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
		{
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		}
		/// <summary>
		/// レンダリングターゲットビューのクリア。
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		/// <param name="clearColor">クリアカラー</param>
		void ClearRenderTargetView(CRenderTargetDx12& renderTarget, const float* clearColor)
		{
			auto rtvHandle = renderTarget.GetRTVCpuDescriptorHandle();
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
		{
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		/// <summary>
		/// デプスステンシルビューをクリア
		/// </summary>
		/// <param name="renderTarget">レンダリングターゲット</param>
		/// <param name="clearValue">クリア値</param>
		void ClearDepthStencilView(CRenderTargetDx12& renderTarget, float clearValue)
		{
			auto dsvHandle = renderTarget.GetDSVCpuDescriptorHandle();
			m_commandList->ClearDepthStencilView(
				dsvHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearValue,
				0,
				0,
				nullptr);
		}
		void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
		{
			m_commandList->ClearDepthStencilView(
				dsvHandle,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
				clearValue,
				0,
				0,
				nullptr);
		}
		/// <summary>
		/// レンダリングターゲットへの描き込み待ち。
		/// </summary>
		/// <remarks>
		/// レンダリングターゲットとして使われているテクスチャをシェーダーリソースビューとして
		/// 使用したい場合は、この関数を使って描き込み完了待ちを行う必要があります。
		/// </remarks>
		/// <param name="renderTarget">レンダリングターゲット</param>
		void WaitUntilFinishDrawingToRenderTarget(CRenderTargetDx12& renderTarget)
		{
			m_commandList->ResourceBarrier(
				1,
				&CD3DX12_RESOURCE_BARRIER::Transition(
					renderTarget.GetRenderTargetTexture().Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_COMMON)
			);
		}
		void WaitUntilFinishDrawingToRenderTarget(ComPtr<ID3D12Resource>& renderTarget)
		{
			m_commandList->ResourceBarrier(
				1,
				&CD3DX12_RESOURCE_BARRIER::Transition(
					renderTarget.Get(),
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PRESENT));
		}
		/// <summary>
		/// レンダリングターゲットとして使用可能になるまで待つ。
		/// </summary>
		/// <remarks>
		/// レンダリングターゲットとして設定したい場合は、
		/// 本関数を使って使用可能になるまで待機する必要があります。
		/// </remarks>
		void WaitUntilToPossibleSetRenderTarget(CRenderTargetDx12& renderTarget)
		{
			//レンダリングターゲットが利用可能になるまでリソースバリア。
			m_commandList->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.GetRenderTargetTexture().Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET)
			);
		}
		void WaitUntilToPossibleSetRenderTarget(ComPtr<ID3D12Resource>& renderTarget)
		{
			m_commandList->ResourceBarrier(
				1,
				&CD3DX12_RESOURCE_BARRIER::Transition(renderTarget.Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET)
			);
		}
		/// <summary>
		/// コマンドリストを閉じる
		/// </summary>
		void Close()
		{
			m_commandList->Close();
		}
		/// <summary>
		/// コマンドリストをリセット。
		/// </summary>
		/// <param name="commandAllocator"></param>
		/// <param name="pipelineState"></param>
		void Reset(ComPtr<ID3D12CommandAllocator>& commandAllocator, ComPtr<ID3D12PipelineState>& pipelineState)
		{
			m_commandList->Reset(commandAllocator.Get(), pipelineState.Get());
		}
	private:
		enum { MAX_DESCRIPTOR_HEAP = 4 };	//ディスクリプタヒープの最大数。
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//コマンドリスト。
		ID3D12DescriptorHeap* m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

