#pragma once

namespace tkEngine {
	class CGraphicsEngineDx12;
	class CTextureDx12 : public ITexture{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CTextureDx12() {}
		/// <summary>
		/// �t�@�C������e�N�X�`�������[�h����R���X�g���N�^
		/// </summary>
		/// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
		explicit CTextureDx12(const wchar_t* filePath);
		/// <summary>
		/// DDS�t�@�C������e�N�X�`��������������B
		/// </summary>
		/// <param name="filePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
		void InitFromDDSFile(const wchar_t* filePath);
		/// <summary>
		/// ����������e�N�X�`��������������B
		/// </summary>
		/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X</param>
		/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
		void InitFromMemory(const char* memory, unsigned int size);
		
		/// <summary>
		/// SRV�ɓo�^�B
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
	private:
		/// <summary>
		/// DDS�t�@�C������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
		/// <param name="device">D3D�f�o�C�X</param>
		void LoadTextureFromDDSFile(
			const wchar_t* filePath, 
			CGraphicsEngineDx12* ge12, 
			ComPtr<ID3D12Device> device
		);
		/// <summary>
		/// ����������e�N�X�`�������[�h�B
		/// </summary>
		/// <param name="memory">�e�N�X�`���f�[�^���i�[����Ă��郁�����̐擪�A�h���X�B</param>
		/// <param name="size">�e�N�X�`���̃T�C�Y�B</param>
		/// <param name="ge12">Dx12�ł̃O���t�B�b�N�X�G���W��</param>
		/// <param name="device">D3D�f�o�C�X</param>
		void LoadTextureFromMemory(
			const char* memory, 
			unsigned int size,
			CGraphicsEngineDx12* ge12,
			ComPtr<ID3D12Device> device
		);
		
	private:
		ComPtr< ID3D12Resource>	m_texture;	//�e�N�X�`���B
		D3D12_RESOURCE_DESC m_textureDesc;	//�e�N�X�`�����
	};
}