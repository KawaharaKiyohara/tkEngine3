#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/Graphics/tkGraphicsInstanceFactory.h"

namespace tkEngine {

	/// <summary>
	/// �O���t�B�b�N�n�̃v���b�g�t�H�[���Ɉˑ�����C���X�^���X�̃t�@�N�g���[�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	/// <remarks>
	/// AbstractFactory�p�^�[���Ő݌v����Ă��܂��B
	/// </remarks>
	class CGraphicsInstanceFactoryDx12 : public IGraphicsInstanceFactory {
	public:
		/// <summary>
		/// �O���t�B�b�N�G���W���̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		UPIGraphicsEngine CreateGraphicsEngineInstance() override final;
		/// <summary>
		/// ���_�o�b�t�@�̃C���X�^���X���쐬�B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		UPIVertexBuffer CreateVertexBufferInstance(int size, int stride) override final;
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�̃C���X�^���X���쐬�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		UPIIndexBuffer CreateIndexBufferInstance(int size, int stride) override final;
		/// <summary>
		/// �����_�����O�R���e�L�X�g�̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		UPIRenderContext CreateRenderContextInstance()override final;
		/// <summary>
		/// �O�p�`�`��̃C���X�^���X���쐬�B
		/// </summary>
		/// /// <param name="textureFilePath">
		/// �O�p�`�ɓ\��t����e�N�X�`���̃t�@�C���p�X�B
		/// �T�|�[�g���Ă���摜�f�[�^��dds�t�@�C���݂̂ł��B
		/// nullptr���w�肵���ꍇ�́A�e�N�X�`�������̎O�p�`���\������܂��B
		/// </param>
		/// <returns></returns>
		UPIShape CreateTriangleShapeInstance(const wchar_t* textureFilePath)override final;
		/// <summary>
		/// DDS�t�@�C������e�N�X�`�����쐬�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <returns>�쐬���ꂽ�e�N�X�`��</returns>
		UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) override final;
	};
}

#endif