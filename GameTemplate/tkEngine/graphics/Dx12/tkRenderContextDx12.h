#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"

namespace tkEngine {
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
		/// �v���~�e�B�u�̃g�|���W�[��ݒ�B
		/// </summary>
		/// <remarks>
		/// ID3D12GraphicsCommandList::��IASetPrimitiveTopology�̃��b�p�[�֐��B
		/// �ڍׂ�Microsoft�̃w���v���Q�ƁB
		/// </remarks>
		void IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
		{
			m_commandList->IASetPrimitiveTopology(topology);
		}
		void IASetPrimitiveTopology(EnPrimitiveTopology topology)
		{
			m_commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
		}
		/// <summary>
		/// ���_�o�b�t�@��ݒ�B
		/// </summary>
		void IASetVertexBuffer(CVertexBufferDx12& vb)
		{
			m_commandList->IASetVertexBuffers(0, 1, &vb.GetView());
		}
		void IASetVertexBuffer(UPIVertexBuffer& vb)
		{
			auto vbDx12 = vb->As<CVertexBufferDx12>();
			m_commandList->IASetVertexBuffers(0, 1, &vbDx12->GetView());
		}
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@��ݒ�B
		/// </summary>
		/// <param name="ib"></param>
		void IASetIndexBuffer(CIndexBufferDx12& ib)
		{
			m_commandList->IASetIndexBuffer(&ib.GetView());
		}
		void IASetIndexBuffer(UPIIndexBuffer& ib)
		{
			auto ibDx12 = ib->As<CIndexBufferDx12>();
			m_commandList->IASetIndexBuffer(&ibDx12->GetView());
		}
		void IASetIndexBuffer(UPCIndexBufferDx12& ib)
		{
			m_commandList->IASetIndexBuffer(&ib->GetView());
		}
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
		/// <summary>
		/// �p�C�v���C���X�e�[�g��ݒ�B
		/// </summary>
		void SetPipelineState(ComPtr<ID3D12PipelineState>& pipelineState)
		{
			m_commandList->SetPipelineState(pipelineState.Get());
		}
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v��ݒ�B
		/// </summary>
		void SetDescriptorHeap(ComPtr< ID3D12DescriptorHeap>& descHeap)
		{
			ID3D12DescriptorHeap* ppHeap[] = { descHeap.Get() };
			m_commandList->SetDescriptorHeaps(1, ppHeap);
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
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//�R�}���h���X�g�B
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

