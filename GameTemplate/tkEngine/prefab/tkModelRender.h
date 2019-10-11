/// <summary>
/// ���f�������_���[�B
/// </summary>
#pragma once

#include "tkEngine/graphics/tkModel.h"

namespace tkEngine {
	namespace prefab {
		/// <summary>
		/// 3D���f����\������@�\��񋟂���N���X�B
		/// </summary>
		class CModelRender : public IGameObject {
		public:
			/// <summary>
			/// �������B
			/// </summary>
			/// <param name="filePath"></param>
			void Init(const char* filePath);
			/// <summary>
			/// �J�n����
			/// </summary>
			/// <returns></returns>
			bool Start() override;
			/// <summary>
			/// �X�V�����B
			/// </summary>
			void Update() override;
			/// <summary>
			/// �t�H���[�h�����_�����O�̃p�X�ŌĂ΂�鏈���B
			/// </summary>
			/// <param name="renderContext"></param>
			void ForwardRender(IRenderContext& renderContext) override;
			/// <summary>
			/// ���f�����ړ�������B
			/// </summary>
			/// <param name="move">�ړ���</param>
			void Move(CVector3 move)
			{
				m_position += move;
			}
			/// <summary>
			/// ���f������]������B
			/// </summary>
			/// <param name="rot">��]��</param>
			void Rotate(CQuaternion rot)
			{
				//��]�����Z����B
				m_rotation *= rot;
			}
			/// <summary>
			/// ���W�𒼐ڐݒ�B
			/// </summary>
			/// <param name="pos">���W</param>
			void SetPosition(CVector3 pos)
			{
				m_position = pos;
			}
			/// <summary>
			/// ��]�𒼐ڐݒ�B
			/// </summary>
			/// <param name="rot"></param>
			void SetRotation(CQuaternion rot)
			{
				m_rotation = rot;
			}
			/// <summary>
			/// �g�嗦�𒼐ڐݒ�B
			/// </summary>
			/// <param name="scale"></param>
			void SetScale(CVector3 scale)
			{
				m_scale = scale;
			}
		private:
			/// <summary>
			/// �������X�e�[�^�X�B
			/// </summary>
			enum EnInitStatus {
				enInitStatus_NotCallInitFunc,	//�������֐����Ă΂�Ă��Ȃ��B
				enInitStatus_WaitInitModel,		//���f���������҂��B
				enInitStatus_Completed,			//�����������B
			};
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//�������X�e�[�^�X�B
			CModel m_model;		//���f���B
			CVector3 m_position = CVector3::Zero;			//!<���W�B
			CQuaternion	m_rotation = CQuaternion::Identity;	//!<��]�B
			CVector3 m_scale = CVector3::One;				//!<�g�嗦�B
			CSkeleton m_skeleton;							//!<�X�P���g���B
		};
	}
}