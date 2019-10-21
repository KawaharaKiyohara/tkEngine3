/*!
 * @brief	���C�g�̊Ǘ��ҁB
 */

#pragma once
#include "tkEngine/graphics/tkLight.h"

namespace tkEngine{
	namespace prefab {
		class CDirectionLight;
		class CPointLight;
	//	class CSpotLight;
		class CLightBase;
	}
	/*!
	 * @brief	���C�g�̊Ǘ��ҁB
	 * @details
	 *  �C���X�^���X�����ꂽCLightBase�̃C���X�^���X�͂��ׂ�CLightManager�ɓo�^����܂��B
	 *  ���̃N���X�̃C���X�^���X��CGraphicsEngine�N���X�݂̂��ێ����Ă���A���̑��̃N���X�ŃC���X�^���X�𐶐����邱�Ƃ͂ł��܂���B
	 */
	class ILightManager{
	friend class CGraphicsEngine;
	
	public:
		void Init();
		/*!
		*@brief�@���C�g��ǉ��B
		*/
		void AddLight(prefab::CLightBase* light);
		/*!
		*@brief�@���C�g���폜�B
		*/
		void RemoveLight(prefab::CLightBase* light);
		/*!
		*@brief�@�X�V�B
		*/
		void Update();

		/// <summary>
		/// �`��V�[�P���X����Ă΂�鏈���B
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		void Render(IRenderContext& renderContext);
#if 0//todo
		/*!
		*@brief�@�P�t���[���̕`��I�����ɌĂ΂�鏈���B
		*/
		void EndRender(CRenderContext& renderContext);
#endif
		/*!
		*@brief�@�|�C���g���C�g�̐����擾�B
		*/
		int GetNumPointLight() const
		{
			return static_cast<int>(m_pointLights.size());
		}

		/*!
		 *@brief	�A���r�G���g���C�g���擾�B
		 */
		const CVector3& GetAmbientLight() const
		{
			return m_lightParam.ambientLight;
		}
		/*!
		 *@brief	�A���r�G���g���C�g��ݒ�B
		 */
		void SetAmbientLight(const CVector3& ambient)
		{
			m_lightParam.ambientLight = ambient;
		}
		/// <summary>
		/// GPU�Ƀ��C�g�̏���]�����܂��B
		/// </summary>
		virtual void SendLightDataToGPU(IRenderContext& rc) = 0;
		/// <summary>
		/// ILightManager�̔h���N���X�Ɍ^�ϊ��B
		/// </summary>
		template<class T>
		T& As()
		{
			return dynamic_cast<T&>(*this);
		}
	private:
		
		/// <summary>
		/// ���������ɌĂ΂��R�[�h�B
		/// </summary>
		virtual void OnInit() = 0;
		/// <summary>
		/// �`�掞�ɌĂ΂��R�[�h�B
		/// </summary>
		virtual void OnRender(IRenderContext& rc) = 0;
	protected:
		static const int MAX_DIRECTION_LIGHT = 8;						//!<�f�B���N�V�������C�g�̍ő吔�B
		static const int MAX_POINT_LIGHT = 1024;						//!<�|�C���g���C�g�̍ő吔�B
		/*!
		 *@brief	GPU�Ŏg�p���郉�C�g�p�̃p�����[�^�B
		 *@details
		 * �����o�ϐ���ǉ�������A���т�ς����肵����
		 * Assets/shader/modelCB.h��lightCb���ς���K�v����B
		 */
		struct SLightParam {
			CVector3 eyePos;			//�����̈ʒu�B
			int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
			CVector3 ambientLight;		//�A���r�G���g���C�g�B
			int numPointLight;			//�|�C���g���C�g�̐��B
			CVector4 screenParam;		//�X�N���[���p�����[�^�B
		};
		SLightParam							m_lightParam;
		SDirectionLight						m_rawDirectionLights[MAX_DIRECTION_LIGHT];
		SPointLight							m_rawPointLights[MAX_POINT_LIGHT];
		std::list<prefab::CDirectionLight*>	m_directionLights;		//!<���s�����̃��X�g�B
		std::list<prefab::CPointLight*>		m_pointLights;			//!<�|�C���g���C�g�̃��X�g�B
	};
}