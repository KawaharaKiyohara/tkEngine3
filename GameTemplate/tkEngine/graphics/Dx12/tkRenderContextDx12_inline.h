#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


namespace tkEngine {
	inline void CRenderContextDx12::SetCBR_SRV_UAV(
		ID3D12DescriptorHeap* descriptorHeap,
		int numConstantBuffer, 
		CConstantBufferDx12* constantBufferArray, 
		int numShaderResource, 
		IShaderResourceDx12* shaderResourceArray[]
	)
	{
		m_commandList->SetDescriptorHeaps(1, &descriptorHeap);

		auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//定数バッファを登録していく。
		for (int i = 0; i < numConstantBuffer; i++) {
			constantBufferArray[i].RegistConstantBufferView(cpuHandle);
			//次に進める。
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}
		//続いてテクスチャ
		for (int i = 0; i < numShaderResource; i++) {
			shaderResourceArray[i]->RegistShaderResourceView(cpuHandle);
			//次に進める。
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		if (numConstantBuffer > 0) {
			//ディスクリプタヒープをディスクリプタテーブルに登録していく。
			SetGraphicsRootDescriptorTable(
				0,
				gpuHandle
			);
		}
		if (numShaderResource > 0) {
			//続いてシェーダーリソース。
			gpuHandle.ptr += (UINT64)ge12.GetCbrSrvDescriptorSize() * numConstantBuffer;
			SetGraphicsRootDescriptorTable(
				1,
				gpuHandle
			);
		}
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

