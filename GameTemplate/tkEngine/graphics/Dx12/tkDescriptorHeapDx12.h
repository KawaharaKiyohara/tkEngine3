#pragma once


namespace tkEngine {
	class IShaderResourceDx12;
	/// <summary>
	/// ディスクリプタヒープ。
	/// </summary>
	class CDescriptorHeapDx12 : Noncopyable {
	private:
		enum {
			MAX_SHADER_RESOURCE = 64,
			MAX_CONSTANT_BUFFER = 64,
		};
	public:
		CDescriptorHeapDx12() {}
		CDescriptorHeapDx12(CDescriptorHeapDx12&& rhs)
		{
			m_descriptorHeap[0] = std::move(rhs.m_descriptorHeap[0]);
			m_descriptorHeap[1] = std::move(rhs.m_descriptorHeap[1]);
		}
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
			if (m_numShaderResource < registerNo + 1) {
				m_numShaderResource = registerNo + 1;
			}
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
			if (m_numConstantBuffer < registerNo + 1) {
				m_numConstantBuffer = registerNo + 1;
			}
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
		CConstantBufferDx12* m_constantBuffers[MAX_SHADER_RESOURCE] = { nullptr };
		int m_numShaderResource = 0;	//シェーダーリソースの数
		int m_numConstantBuffer = 0;	//定数バッファの数。

		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];		//定数バッファのディスクリプタヒープの開始ハンドル。
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];		//シェーダーリソースのディスクリプタヒープの開始ハンドル。
	};
}