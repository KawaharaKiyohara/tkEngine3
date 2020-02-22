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
		/// �O���t�B�b�N�G���W�����쐬�B
		/// </summary>
		/// <returns></returns>
		UPIGraphicsEngine CreateGraphicsEngine() override final;
		/// <summary>
		/// ���_�o�b�t�@�̍쐬�B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		UPIVertexBuffer CreateVertexBuffer(int size, int stride) override final;
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@���쐬�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�X�g���C�h</param>
		/// <returns></returns>
		UPIIndexBuffer CreateIndexBuffer(int size, int stride) override final;
		/// <summary>
		/// �����_�����O�R���e�L�X�g���쐬�B
		/// </summary>
		/// <returns></returns>
		UPIRenderContext CreateRenderContext()override final;
		/// <summary>
		/// DDS�t�@�C������e�N�X�`�����쐬�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		/// <returns>�쐬���ꂽ�e�N�X�`��</returns>
		UPITexture CreateTextureFromDDSFile(const wchar_t* filePath) override final;
		/// <summary>
		/// TKM�t�@�C�����烁�b�V���p�[�c���쐬�B
		/// </summary>
		/// <param name="tkmFile">tkm�t�@�C��</param>
		/// <returns>�쐬���ꂽ���b�V���p�[�c�B</returns>
		UPIMeshParts CreateMeshPartsFromTkmFile(const CTkmFile& tkmFile) override final;
		/// <summary>
		/// ���C�g�}�l�[�W���[���쐬�B
		/// </summary>
		/// <returns>�쐬���ꂽ���C�g�}�l�[�W���B</returns>
		UPILightManager CreateLightManager() override final;
		/// <summary>
		/// �w�����V���h�E�}�b�v�̃C���X�^���X���쐬�B
		/// </summary>
		/// <returns></returns>
		UPIDirectionalShadowMap CreateDirectionalShadowMap() override final;

	};
}

#endif