#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


namespace tkEngine {
	inline void CRenderContextDx12::DrawCommon()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		int numDescriptor = MAX_CONSTANT_BUFFER + MAX_SHADER_RESOURCE;
		auto descriptorHeap = ge12.AllocDescriptorHeap(numDescriptor).Get();
		m_commandList->SetDescriptorHeaps(1, &descriptorHeap);

		auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

		//定数バッファを登録していく。
		for (auto& cb : m_constantBuffers) {
			if (cb != nullptr) {
				cb->RegistConstantBufferView(cpuHandle);
			}
			//次に進める。
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		//続いてシェーダーリソース。
		for (auto& srv : m_shaderResources) {
			if (srv != nullptr) {
				srv->RegistShaderResourceView(cpuHandle);
			}
			//次に進める。
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		//ディスクリプタヒープをディスクリプタテーブルに登録していく。
		SetGraphicsRootDescriptorTable(
			0,
			gpuHandle
		);
		//続いてシェーダーリソース。
		gpuHandle.ptr += (UINT64)ge12.GetCbrSrvDescriptorSize() * MAX_CONSTANT_BUFFER;
		SetGraphicsRootDescriptorTable(
			1,
			gpuHandle
		);

	}
#if 0
	inline void CRenderContextDx12::SetCBR_SRV_UAV(
		CConstantBufferDx12* constantBufferArray[],
		IShaderResourceDx12* shaderResourceArray[],
		int numConstantBuffer,
		int numShaderResource
	)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		int numDescriptor = numConstantBuffer + numShaderResource;
		auto descriptorHeap = ge12.AllocDescriptorHeap(numDescriptor).Get();

		m_commandList->SetDescriptorHeaps(1, &descriptorHeap);

		auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		
		//定数バッファを登録していく。
		for (int i = 0; i < numConstantBuffer; i++) {
			if (constantBufferArray[i] != nullptr) {
				constantBufferArray[i]->RegistConstantBufferView(cpuHandle);
			}
			//次に進める。
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}
		//続いてテクスチャ
		for (int i = 0; i < numShaderResource; i++) {
			if (shaderResourceArray[i] != nullptr) {
				shaderResourceArray[i]->RegistShaderResourceView(cpuHandle);
			}
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
#endif
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

