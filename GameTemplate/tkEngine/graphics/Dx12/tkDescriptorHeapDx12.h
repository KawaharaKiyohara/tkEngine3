#pragma once

namespace tkEngine {
	/// <summary>
	/// ディスクリプタヒープ。
	/// </summary>
	class CDescriptorHeapDx12 : Noncopyable {
	public:
		/// <summary>
		/// ディスクリプタヒープの初期化。
		/// </summary>
		/// <param name="type">ヒープの種類。</param>
		/// <param name="numDescriptor">ディスクリプタの数。</param>
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
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap;	//ディスクリプタヒープ。
	};
}