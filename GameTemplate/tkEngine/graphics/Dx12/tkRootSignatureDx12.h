#pragma once

namespace tkEngine {
	class CRootSignatureDx12 : Noncopyable {
	public:
		
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="samplerFilter">�T���v���t�B���^</param>
		/// <param name="textureAdressModeU">�e�N�X�`���̃A�h���b�V���O���[�h(U����)</param>
		/// <param name="textureAdressModeV">�e�N�X�`���̃A�h���b�V���O���[�h(V����)</param>
		/// <param name="textureAdressModeW">�e�N�X�`���̃A�h���b�V���O���[�h(W����)</param>
		/// <param name="maxCbvDescriptor">�萔�o�b�t�@�p�̃f�B�X�N���v�^�̍ő吔</param>
		/// <param name="maxSrvDescriptor">�V�F�[�_�[���\�[�X�p�̃f�B�X�N���v�^�̍ő吔</param>
		bool Init(
			D3D12_FILTER samplerFilter,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeU,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeV,
			D3D12_TEXTURE_ADDRESS_MODE textureAdressModeW,
			UINT maxCbvDescriptor = 8,
			UINT maxSrvDescriptor = 32
		);
		ID3D12RootSignature* Get()
		{
			return m_rootSignature.Get();
		}
	private:
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
	};
}