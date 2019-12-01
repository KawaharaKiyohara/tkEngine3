#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

namespace tkEngine {
	class CShaderDx12 {
	public:
		/// <summary>
		/// �V�F�[�_�[�����[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
		/// <param name="shaderModel">�V�F�[�_�\���f���B</param>
		void Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel);
		void LoadPS(const wchar_t* filePath, const char* entryFuncName);
		void LoadVS(const wchar_t* filePath, const char* entryFuncName);
		/// <summary>
		/// �R���p�C���ς݃V�F�[�_�[�f�[�^���擾�B
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3DBlob> GetCompiledBlob() const
		{
			return m_blob;
		}
	private:
		ComPtr<ID3DBlob>	m_blob;	//�R���p�C���ς݂̃V�F�[�_�[�f�[�^�B
	};

	using UPIShape = std::unique_ptr<IShape>;
}

#endif