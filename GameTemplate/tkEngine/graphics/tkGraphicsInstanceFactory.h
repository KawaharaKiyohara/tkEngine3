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
		/// <returns></returns>
		virtual UPIShape CreateTriangleShapeInstance() = 0;
	};
}