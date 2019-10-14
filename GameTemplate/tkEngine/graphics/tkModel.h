#pragma once

#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/tkMeshParts.h"
#include "tkEngine/graphics/tkSkeleton.h"

namespace tkEngine {
	/// <summary>
	/// ���f���N���X�B
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// tkm�t�@�C����񓯊����\�h�B
		/// </summary>
		/// <remarks>
		/// IsInited�֐��𗘗p���āA�������Ƃ�悤�ɂ��Ă��������B
		/// </remarks>
		/// <param name="filePath">tkm�t�@�C���̃t�@�C���p�X�B</param>
		void LoadTkmFileAsync(const char* filePath);
		/// <summary>
		/// tkm�t�@�C�������[�h�B
		/// </summary>
		/// <param name="filePath">tkm�t�@�C���̃t�@�C���p�X�B</param>
		void LoadTkmFile(const char* filePath);
		/// <summary>
		/// ���b�V���p�[�c���쐬����B
		/// </summary>
		/// <remarks>
		/// ���̊֐���LoadTkmFileAsync�֐��Ȃǂ𗘗p���āA
		/// tkm�t�@�C�������[�h������ŌĂяo���Ă��������B
		/// </remarks>
		void CreateMeshParts();
		/// <summary>
		/// ���������I����Ă��邩����B
		/// </summary>
		/// <returns></returns>
		bool IsInited() const;
		/// <summary>
		/// �X�V�B
		/// �K�����t���[�����s���Ă��������B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		void Update(CVector3 pos, CQuaternion rot, CVector3 scale);
		/// <summary>
		/// �X�P���g�����֘A�t����B
		/// </summary>
		/// <param name="skeleton">�X�P���g��</param>
		void BindSkeleton(CSkeleton& skeleton)
		{
			m_meshParts->BindSkeleton(skeleton);
		}
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��</param>
		void Draw(IRenderContext& rc, CMatrix mView, CMatrix mProj);
		/// <summary>
		/// ���[���h�s����擾�B
		/// </summary>
		/// <returns></returns>
		const CMatrix& GetWorldMatrix() const
		{
			return m_world;
		}
	private:
		CMatrix m_world;			//���[���h�s��B
		CTkmFile m_tkmFile;			//tkm�t�@�C���B
		CSkeleton m_skeleton;		//�X�P���g���B
		UPIMeshParts m_meshParts;	//���b�V���p�[�c�B
	};
}