#pragma once

namespace tkEngine {
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�B
	/// </summary>
	class CDescriptorHeapDx12 : Noncopyable {
	public:
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̏������B
		/// </summary>
		/// <param name="type">�q�[�v�̎�ށB</param>
		/// <param name="numDescriptor">�f�B�X�N���v�^�̐��B</param>
		void Init(D3D12_DESCRIPTOR_HEAP_TYPE type, int numDescriptor);
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ID3D12DescriptorHeap* Get()
		{
			return m_descriptorHeap.Get();
		}
	private:
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap;	//�f�B�X�N���v�^�q�[�v�B
	};
}