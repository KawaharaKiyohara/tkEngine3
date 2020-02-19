#pragma once


namespace tkEngine {
	/// <summary>
	/// ディスクリプタヒープ。
	/// </summary>
	class CDescriptorHeapDx12 : Noncopyable {
	public:
		CDescriptorHeapDx12() {}
		CDescriptorHeapDx12(CDescriptorHeapDx12&& rhs)
		{
			m_descriptorHeap[0] = std::move(rhs.m_descriptorHeap[0]);
			m_descriptorHeap[1] = std::move(rhs.m_descriptorHeap[1]);
		}
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
		ID3D12DescriptorHeap* Get();
		/// <summary>
		/// シェーダーリソースをディスクリプタヒープに登録。
		/// </summary>
		/// <param name="registerNo">レジスタ番号</param>
		/// <param name="sr">シェーダーリソース</param>
		void RegistShaderResource(int registerNo, IShaderResourceDx12& sr)
		{
			TK_ASSERT(registerNo < MAX_SHADER_RESOURCE, "registerNo is invalid");
			m_shaderResources[registerNo] = &sr;
		}
		/// <summary>
		/// 定数バッファをディスクリプタヒープに登録。
		/// </summary>
		/// <param name="registerNo">レジスタ番号</param>
		/// <param name="cb">呈すバッファ</param>
		void RegistConstantBuffer(int registerNo, CConstantBufferDx12& cb)
		{
			TK_ASSERT(registerNo < MAX_CONSTANT_BUFFER, "registerNo is invalid");
			m_constantBuffers[registerNo] = &cb;
		}
		/// <summary>
		/// ディスクリプタヒープへの登録を確定。
		/// </summary>
		void Commit();
		/// <summary>
		/// 定数バッファのディスクリプタの開始ハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescritorStartHandle() const;
		/// <summary>
		/// シェーダーリソースのディスクリプタの開始ハンドルを取得。
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescritorStartHandle() const;
	private:
	
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap[2];	//ディスクリプタヒープ。
		IShaderResourceDx12* m_shaderResources[MAX_SHADER_RESOURCE] = { nullptr };
		CConstantBufferDx12* m_constantBuffers[MAX_CONSTANT_BUFFER] = { nullptr };
		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];		//定数バッファのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];		//シェーダーリソースのディスクリプタヒープの開始ハンドル。
	};
}