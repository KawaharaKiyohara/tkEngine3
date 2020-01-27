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

		//�萔�o�b�t�@��o�^���Ă����B
		for (auto& cb : m_constantBuffers) {
			if (cb != nullptr) {
				cb->RegistConstantBufferView(cpuHandle);
			}
			//���ɐi�߂�B
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		//�����ăV�F�[�_�[���\�[�X�B
		for (auto& srv : m_shaderResources) {
			if (srv != nullptr) {
				srv->RegistShaderResourceView(cpuHandle);
			}
			//���ɐi�߂�B
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}

		//�f�B�X�N���v�^�q�[�v���f�B�X�N���v�^�e�[�u���ɓo�^���Ă����B
		SetGraphicsRootDescriptorTable(
			0,
			gpuHandle
		);
		//�����ăV�F�[�_�[���\�[�X�B
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
		
		//�萔�o�b�t�@��o�^���Ă����B
		for (int i = 0; i < numConstantBuffer; i++) {
			if (constantBufferArray[i] != nullptr) {
				constantBufferArray[i]->RegistConstantBufferView(cpuHandle);
			}
			//���ɐi�߂�B
			cpuHandle.ptr += ge12.GetCbrSrvDescriptorSize();
		}
		//�����ăe�N�X�`��
		for (int i = 0; i < numShaderResource; i++) {
			if (shaderResourceArray[i] != nullptr) {
				shaderResourceArray[i]->RegistShaderResourceView(cpuHandle);
			}
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
#endif
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

