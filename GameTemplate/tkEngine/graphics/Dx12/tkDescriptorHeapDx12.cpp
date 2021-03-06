#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDescriptorHeapDx12.h"


namespace tkEngine {

	ID3D12DescriptorHeap* CDescriptorHeapDx12::Get()
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_descriptorHeap[backBufferIndex].Get();
	}
	D3D12_GPU_DESCRIPTOR_HANDLE CDescriptorHeapDx12::GetConstantBufferGpuDescritorStartHandle() const
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_cbGpuDescriptorStart[backBufferIndex];
	}
	D3D12_GPU_DESCRIPTOR_HANDLE CDescriptorHeapDx12::GetShaderResourceGpuDescritorStartHandle() const
	{
		auto& geDx12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto backBufferIndex = geDx12.GetBackBufferIndex();
		return m_srGpuDescriptorStart[backBufferIndex];
	}
	void CDescriptorHeapDx12::Commit()
	{
		//ディスクリプタヒープを作成。
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = m_numShaderResource + m_numConstantBuffer;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap[0]));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");
		hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap[1]));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");

		//定数バッファやシェーダーリソースのディスクリプタをヒープに書き込んでいく。
		int bufferNo = 0;
		for (auto& descriptorHeap : m_descriptorHeap) {
			auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

			//定数バッファを登録していく。
			for( int i = 0; i < m_numConstantBuffer; i++ ){
				if (m_constantBuffers[i] != nullptr) {
					m_constantBuffers[i]->RegistConstantBufferView(cpuHandle, bufferNo);
				}
				//次に進める。
				cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
			}

			//続いてシェーダーリソース。
			for( int i = 0; i < m_numShaderResource; i++){
				if (m_shaderResources[i] != nullptr) {
					m_shaderResources[i]->RegistShaderResourceView(cpuHandle, bufferNo);
				}
				//次に進める。
				cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
			}

			//定数バッファのディスクリプタヒープの開始ハンドルを計算。
			m_cbGpuDescriptorStart[bufferNo] = gpuHandle;
			//シェーダーリソースのディスクリプタヒープの開始ハンドルを計算。
			m_srGpuDescriptorStart[bufferNo] = gpuHandle;
			m_srGpuDescriptorStart[bufferNo].ptr += (UINT64)ge12.GetCbrSrvDescriptorSize() * m_numConstantBuffer;
			bufferNo++;
		}
	}
}