#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"
#include "tkEngine/graphics/Dx12/tkDescriptorHeapDx12.h"
#include "tkEngine/graphics/Dx12/tkPipelineStateDx12.h"

namespace tkEngine {
	class CConstantBufferDx12;
	class CTextureDx12;
	/// <summary>
	/// �����_�����O�R���e�L�X�g�B
	/// </summary>
	class CRenderContextDx12 : public IRenderContext {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="commandList">�R�}���h���X�g�B</param>
		void Init(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}

		/// <summary>
		/// �R�}���h���X�g��ݒ�B
		/// </summary>
		/// <param name="commandList">�R�}���h���X�g�B</param>
		void SetCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
		/// <summary>
		/// �r���[�|�[�g��ݒ�
		/// </summary>
		/// <param name="viewport">�r���[�|�[�g</param>
		void SetViewport(D3D12_VIEWPORT& viewport)
		{
			m_commandList->RSSetViewports(1, &viewport);
		}
		/// <summary>
		/// �V�U�����O��`��ݒ�
		/// </summary>
		/// <param name="rect"></param>
		void SetScissorRect(D3D12_RECT& rect)
		{
			m_commandList->RSSetScissorRects(1, &rect);
		}
		/// <summary>
		/// �v���~�e�B�u�̃g�|���W�[��ݒ�B
		/// </summary>
		/// <remarks>
		/// ID3D12GraphicsCommandList::��IASetPrimitiveTopology�̃��b�p�[�֐��B
		/// �ڍׂ�Microsoft�̃w���v���Q�ƁB
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
		/// ���_�o�b�t�@��ݒ�B
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
		/// �C���f�b�N�X�o�b�t�@��ݒ�B
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
		/// �萔�o�b�t�@�A�V�F�[�_�[���\�[�X�AUAV(UnorderResrouceView)���f�B�X�N���v�^�q�[�v�ɓo�^����B
		/// </summary>
		/// <param name="constantBufferArray">�萔�o�b�t�@�̔z��</param>
		/// <param name="numSRV">�V�F�[�_�[���\�[�X�̐�</param>
		/// /// <param name="numCBR">�萔�o�b�t�@�̐�</param>
		/// <param name="srvArray">�V�F�[�_�[���\�[�X�̔z��</param>
		void SetCBR_SRV_UAV(
			CConstantBufferDx12* constantBufferArray[],
			IShaderResourceDx12* shaderResourceArray[],
			int numConstantBuffer,
			int numShaderResource
		);
		/// <summary>
		/// �C���f�b�N�X���v���~�e�B�u��`��B
		/// </summary>
		/// <param name="indexCount">�C���f�b�N�X�̐��B</param>
		void DrawIndexed(UINT indexCount)
		{
			m_commandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
		}
		/// <summary>
		/// ���[�g�V�O�l�`����ݒ�B
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
		/// �p�C�v���C���X�e�[�g��ݒ�B
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
		/// �f�B�X�N���v�^�q�[�v��ݒ�B
		/// </summary>
		void SetDescriptorHeap(ComPtr< ID3D12DescriptorHeap>& descHeap)
		{
			m_descriptorHeaps[0] = descHeap.Get();	//�J���J���J��
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
		/// �����_�����O�^�[�Q�b�g�ƃr���[�|�[�g�𓯎��ɐݒ肷��B
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
		/// �����_�����O�^�[�Q�b�g��ݒ�
		/// </summary>
		void SetRenderTarget(CRenderTargetDx12& renderTarget)
		{
			auto rtvHandle = renderTarget.GetRTVCpuDescriptorHandle();
			if (renderTarget.IsExsitDepthStencilBuffer() ){
				auto dsvHandle = renderTarget.GetDSVCpuDescriptorHandle();
				m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
			}
			else {
				//�f�v�X�X�e���V���o�b�t�@�͂Ȃ��B
				m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
			}
		}
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
		{
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		}
		/// <summary>
		/// �����_�����O�^�[�Q�b�g�r���[�̃N���A�B
		/// </summary>
		/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
		/// <param name="clearColor">�N���A�J���[</param>
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
		/// �f�v�X�X�e���V���r���[���N���A
		/// </summary>
		/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
		/// <param name="clearValue">�N���A�l</param>
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
		/// �����_�����O�^�[�Q�b�g�ւ̕`�����ݑ҂��B
		/// </summary>
		/// <remarks>
		/// �����_�����O�^�[�Q�b�g�Ƃ��Ďg���Ă���e�N�X�`�����V�F�[�_�[���\�[�X�r���[�Ƃ���
		/// �g�p�������ꍇ�́A���̊֐����g���ĕ`�����݊����҂����s���K�v������܂��B
		/// </remarks>
		/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
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
		/// �����_�����O�^�[�Q�b�g�Ƃ��Ďg�p�\�ɂȂ�܂ő҂B
		/// </summary>
		/// <remarks>
		/// �����_�����O�^�[�Q�b�g�Ƃ��Đݒ肵�����ꍇ�́A
		/// �{�֐����g���Ďg�p�\�ɂȂ�܂őҋ@����K�v������܂��B
		/// </remarks>
		void WaitUntilToPossibleSetRenderTarget(CRenderTargetDx12& renderTarget)
		{
			//�����_�����O�^�[�Q�b�g�����p�\�ɂȂ�܂Ń��\�[�X�o���A�B
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
		/// �R�}���h���X�g�����
		/// </summary>
		void Close()
		{
			m_commandList->Close();
		}
		/// <summary>
		/// �R�}���h���X�g�����Z�b�g�B
		/// </summary>
		/// <param name="commandAllocator"></param>
		/// <param name="pipelineState"></param>
		void Reset(ComPtr<ID3D12CommandAllocator>& commandAllocator, ComPtr<ID3D12PipelineState>& pipelineState)
		{
			m_commandList->Reset(commandAllocator.Get(), pipelineState.Get());
		}
	private:
		enum { MAX_DESCRIPTOR_HEAP = 4 };	//�f�B�X�N���v�^�q�[�v�̍ő吔�B
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//�R�}���h���X�g�B
		ID3D12DescriptorHeap* m_descriptorHeaps[MAX_DESCRIPTOR_HEAP];
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

