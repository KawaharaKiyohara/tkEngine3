#pragma once


namespace tkEngine {
	class IShaderResourceDx12;
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�B
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
		/// �V�F�[�_�[���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^�B
		/// </summary>
		/// <param name="registerNo">���W�X�^�ԍ�</param>
		/// <param name="sr">�V�F�[�_�[���\�[�X</param>
		void RegistShaderResource(int registerNo, IShaderResourceDx12& sr)
		{
			TK_ASSERT(registerNo < MAX_SHADER_RESOURCE, "registerNo is invalid");
			m_shaderResources[registerNo] = &sr;
			if (m_numShaderResource < registerNo + 1) {
				m_numShaderResource = registerNo + 1;
			}
		}
		/// <summary>
		/// �萔�o�b�t�@���f�B�X�N���v�^�q�[�v�ɓo�^�B
		/// </summary>
		/// <param name="registerNo">���W�X�^�ԍ�</param>
		/// <param name="cb">�悷�o�b�t�@</param>
		void RegistConstantBuffer(int registerNo, CConstantBufferDx12& cb)
		{
			TK_ASSERT(registerNo < MAX_CONSTANT_BUFFER, "registerNo is invalid");
			m_constantBuffers[registerNo] = &cb;
			if (m_numConstantBuffer < registerNo + 1) {
				m_numConstantBuffer = registerNo + 1;
			}
		}
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�ւ̓o�^���m��B
		/// </summary>
		void Commit();
		/// <summary>
		/// �萔�o�b�t�@�̃f�B�X�N���v�^�̊J�n�n���h�����擾�B
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescritorStartHandle() const;
		/// <summary>
		/// �V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�̊J�n�n���h�����擾�B
		/// </summary>
		/// <returns></returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescritorStartHandle() const;
	private:
	
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap[2];	//�f�B�X�N���v�^�q�[�v�B
		IShaderResourceDx12* m_shaderResources[MAX_SHADER_RESOURCE] = { nullptr };
		CConstantBufferDx12* m_constantBuffers[MAX_SHADER_RESOURCE] = { nullptr };
		int m_numShaderResource = 0;	//�V�F�[�_�[���\�[�X�̐�
		int m_numConstantBuffer = 0;	//�萔�o�b�t�@�̐��B

		D3D12_GPU_DESCRIPTOR_HANDLE m_cbGpuDescriptorStart[2];		//�萔�o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
		D3D12_GPU_DESCRIPTOR_HANDLE m_srGpuDescriptorStart[2];		//�V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
	};
}