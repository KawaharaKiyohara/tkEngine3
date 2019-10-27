#pragma once

#include "tkEngine/graphics/tkIndexbuffer.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// �X�v���C�g�N���X�B
	/// </summary>
	class ISprite : Noncopyable {
	public:
		static const CVector2	DEFAULT_PIVOT;					//!<�s�{�b�g�B
		virtual ~ISprite();
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="w">��</param>
		/// <param name="h">����</param>
		void Init(ITexture* texture, float w, float h);
		/// <summary>
		/// �X�V�B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		/// <param name="pivot">
		/// �s�{�b�g
		/// 0.5, 0.5�ŉ摜�̒��S����_�B
		/// 0.0, 0.0�ŉ摜�̍����B
		/// 1.0, 1.0�ŉ摜�̉E��B
		/// Unity��uGUI�ɏ����B
		/// </param>
		void Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot = DEFAULT_PIVOT);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		/// <param name="viewMatrix">�r���[�s��</param>
		/// <param name="projMatrix">�v���W�F�N�V�����s��</param>
		void Draw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
	private:
		/// <summary>
		/// ���������ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="w">��</param>
		/// <param name="h">����</param>
		virtual void OnInit(ITexture* texture, float w, float h) = 0;
		/// <summary>
		/// �X�V���ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		/// <param name="pivot">
		/// �s�{�b�g
		/// 0.5, 0.5�ŉ摜�̒��S����_�B
		/// 0.0, 0.0�ŉ摜�̍����B
		/// 1.0, 1.0�ŉ摜�̉E��B
		/// Unity��uGUI�ɏ����B
		/// </param>
		virtual void OnUpdate(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot ) = 0;
		/// <summary>
		/// �`�掞�ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		/// <param name="viewMatrix">�r���[�s��</param>
		/// <param name="projMatrix">�v���W�F�N�V�����s��</param>
		virtual void OnDraw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix) = 0;
	protected:
		UPIIndexBuffer m_indexBuffer;				//�C���f�b�N�X�o�b�t�@�B
		UPIVertexBuffer m_vertexBuffer;				//���_�o�b�t�@�B
		ITexture* m_texture = nullptr;
		CVector3 m_position = g_vec3Zero;			//���W�B
		CVector2 m_size = g_vec2Zero;				//�T�C�Y�B
		CQuaternion m_rotation = g_quatIdentity;	//��]�B
		CMatrix m_world = g_matIdentity;			//���[���h�s��B
	};
}