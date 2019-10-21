/*!
 * @brief	���C�g�̊Ǘ��ҁB
 */

#pragma once
#include "tkEngine/graphics/tkLightManager.h"

namespace tkEngine{
	
	/*!
	 * @brief	DirectX12�ł̃��C�g�̊Ǘ��ҁB
	 */
	/// <summary>
	/// DirectX12�ł̃��C�g�̊Ǘ��ҁB
	/// </summary>

	class CLightManagerDx12 : public ILightManager{	
	public:
		/// <summary>
		/// ���ʃ��C�g�p�����[�^�̒萔�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		CConstantBufferDx12& GetLightParamConstantBuffer()
		{
			return m_lightParamCB;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̃X�g���N�`���[�o�b�t�@���擾�B
		/// </summary>
		/// <returns></returns>
		CStructuredBufferDx12& GetDirectionLightStructuredBuffer()
		{
			return m_directionLightSB;
		}
	private:
		/// <summary>
		/// GPU�Ƀ��C�g�̏���]�����܂��B
		/// </summary>
		void SendLightDataToGPU(IRenderContext& rc) override final;
		/// <summary>
		/// ���������ɌĂ΂�鏈���B
		/// </summary>
		void OnInit() override final;
		/// <summary>
		/// �`�掞�ɌĂ΂��R�[�h�B
		/// </summary>
		void OnRender(IRenderContext& rc) override final;
	private:
		/// <summary>
		/// ���f���V�F�[�_�[�Ŏg�p���郉�C�g�p�̃p�����[�^�B
		/// </summary>
		/// <remarks>
		/// �����o�ϐ���ǉ�������A���т�ς����肵����
		/// Assets / shader / modelCB.h��lightCb���ς���K�v����B
		/// </remarks>
		struct SLightParam {
			CVector3 eyePos;			//�����̈ʒu�B
			int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
			CVector3 ambientLight;		//�A���r�G���g���C�g�B
			int numPointLight;			//�|�C���g���C�g�̐��B
			CVector4 screenParam;		//�X�N���[���p�����[�^�B
		};
		CConstantBufferDx12 m_lightParamCB;			//�萔�o�b�t�@�B
		CStructuredBufferDx12 m_directionLightSB;	//�f�B���N�V�������C�g�̃X�g���N�`���o�b�t�@�B
	};
}