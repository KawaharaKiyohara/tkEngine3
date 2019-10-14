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
		//�萔�o�b�t�@��o�^���Ă����B
		for (int i = 0; i < numConstantBuffer; i++) {
			constantBufferArray[i].RegistConstantBufferView(cpuHandle);
			//���ɐi�߂�B
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}
		//�����ăe�N�X�`��
		for (int i = 0; i < numShaderResource; i++) {
			shaderResourceArray[i]->RegistShaderResourceView(cpuHandle);
			//���ɐi�߂�B
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		if (numConstantBuffer > 0) {
			//�f�B�X�N���v�^�q�[�v���f�B�X�N���v�^�e�[�u���ɓo�^���Ă����B
			SetGraphicsRootDescriptorTable(
				0,
				gpuHandle
			);
		}
		if (numShaderResource > 0) {
			//�����ăV�F�[�_�[���\�[�X�B
			gpuHandle.ptr += (UINT64)ge12.GetCbrSrvDescriptorSize() * numConstantBuffer;
			SetGraphicsRootDescriptorTable(
				1,
				gpuHandle
			);
		}
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

