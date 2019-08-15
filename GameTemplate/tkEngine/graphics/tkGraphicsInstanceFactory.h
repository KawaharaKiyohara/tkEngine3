#pragma once


namespace tkEngine {
	
	/// <summary>
	/// �O���t�B�b�N�n�̃v���b�g�t�H�[���Ɉˑ�����C���X�^���X�̃t�@�N�g���[�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	/// <remarks>
	/// AbstractFactory�p�^�[���Ő݌v����Ă��܂��B
	/// </remarks>
	class IGraphicsInstanceFactory {
	public:
		/// <summary>
		/// �t�@�N�g���̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		static UPIGraphicsInstanceFactory CreateInstance();
		/// <summary>
		/// �O���t�B�b�N�G���W���̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		virtual UPIGraphicsEngine CreateGraphicsEngineInstance() = 0;
		/// <summary>
		/// ���_�o�b�t�@�̃C���X�^���X���쐬�B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		virtual UPIVertexBuffer CreateVertexBufferInstance(int size, int stride) = 0;
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�̃C���X�^���X���쐬�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		virtual UPIIndexBuffer CreateIndexBufferInstance(int size, int stride) = 0;
		/// <summary>
		/// �����_�����O�R���e�L�X�g�̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		virtual UPIRenderContext CreateRenderContextInstance() = 0;
		/// <summary>
		/// �O�p�`�`��̃C���X�^���X���쐬�B
		/// </summary>
		/// <param name="textureFilePath">
		/// �O�p�`�ɓ\��t����e�N�X�`���̃t�@�C���p�X�B
		/// �T�|�[�g���Ă���摜�f�[�^��dds�t�@�C���݂̂ł��B
		/// nullptr���w�肵���ꍇ�́A�e�N�X�`�������̎O�p�`���\������܂��B
		/// </param>
		/// <returns></returns>
		virtual UPIShape CreateTriangleShapeInstance(const wchar_t* textureFilePath ) = 0;
		/// <summary>
		/// DDS�t�@�C������e�N�X�`�����쐬�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <returns>�쐬���ꂽ�e�N�X�`��</returns>
		virtual UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) = 0;
	};
}