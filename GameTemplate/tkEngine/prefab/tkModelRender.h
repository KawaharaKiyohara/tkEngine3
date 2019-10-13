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
			/// <remarks>
			/// template�̍����p���g���āA�z��̃T�C�Y�𒲂ׂ���悤�ɂ��Ă܂��B
			/// �댯�A�}�l����ȁB
			/// </remarks>
			/// <param name="tkmFilePath">tkm�t�@�C���̃p�X</param>
			/// <param name="tkaFilePaths">tka�t�@�C���̃p�X�̔z��</param>
			template<typename TKA_FILE_ARRAY, std::size_t NUM_TKA_FILE>
			void Init(const char* tkmFilePath, TKA_FILE_ARRAY (&tkaFilePaths)[NUM_TKA_FILE])
			{
				m_tkmFilePath = tkmFilePath;
				m_model.LoadTkmFileAsync(tkmFilePath);
				for (auto i = 0; i < NUM_TKA_FILE; i++) {
					m_tkaFilePaths.push_back(tkaFilePaths[i]);
				}
				//�������X�e�[�^�X�����f���������҂��ɂ���B
				m_initStatus = enInitStatus_WaitInitModel;
			}
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
				enInitStatus_NotCallInitFunc,			//�������֐����Ă΂�Ă��Ȃ��B
				enInitStatus_WaitInitModel,				//���f���������҂��B
				enInitStatus_WaitInitSkeleton,			//�X�P���g���̏������҂��B
				enInitStatus_WaitInitAnimationClips,	//�A�j���[�V�����N���b�v�̏������҂��B
				enInitStatus_Completed,					//�����������B
			};
			using CUPAnimationClip = unique_ptr<CAnimationClip>;
			EnInitStatus m_initStatus = enInitStatus_NotCallInitFunc;	//�������X�e�[�^�X�B
			CModel m_model;		//���f���B
			CVector3 m_position = g_vec3Zero;			//���W�B
			CQuaternion	m_rotation = g_quatIdentity;	//��]�B
			CVector3 m_scale = g_vec3One;				//�g�嗦�B
			CSkeleton m_skeleton;						//�X�P���g���B
			string m_tkmFilePath;						//tkm�t�@�C���̃t�@�C���p�X�B
			vector< string > m_tkaFilePaths;			//tka�t�@�C���̃t�@�C���p�X�̃��X�g�B
			vector< CUPAnimationClip> m_animationClips;	//�A�j���[�V�����N���b�v�̔z��B
		};
	}
}