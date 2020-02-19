#pragma once

namespace tkEngine {
	/// <summary>
	/// �V�F�[�_�[���\�[�X�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	class  IShaderResourceDx12 : Noncopyable {
	public:
		/// <summary>
		/// SRV�ɓo�^�B
		/// </summary>
		/// <param name="descriptorHandle"></param>
		virtual void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) = 0;
		virtual void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo) = 0;
	};
}