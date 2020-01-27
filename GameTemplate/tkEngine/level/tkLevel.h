/*!
 *@brief	���x���B
 */

#pragma once
#include "tkTklFile.h"

namespace tkEngine{
	class CMapChip;
	class CMapChipRender;
	/*!
	* @brief	�I�u�W�F�N�g���B
	*/
	struct LevelObjectData {
		CVector3 position;		//<���W�B
		CQuaternion rotation;	//!<��]�B
		CVector3 scale;			//!<�g�嗦�B
		const wchar_t* name;	//!<���O�B
		bool isShadowCaster; //!<�V���h�E�L���X�^�[
		bool isShadowReceiver; //!<�V���h�E���V�[�o�[
		/*!
		* @brief	�����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�����ׂ�B
		*@param[in]	objName		���ׂ閼�O�B
		*@return	���O�������ꍇ��true��Ԃ��܂��B
		*/
		bool EqualObjectName(const wchar_t* objName)
		{
			return wcscmp(objName, name) == 0;
		}
		/*!
		* @brief	���O���O����v���邩���ׂ�B
		*/
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//���O�������B�s��v�B
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}
	};
	/*!
	 *@brief	���x���B
	 */
	class CLevel : Noncopyable{
	private:
		using CMapChipPtr = std::unique_ptr<CMapChip>;
		using CMapChipRenderPtr = std::unique_ptr<CMapChipRender>;
	public:
		~CLevel();
		/*!
		 * @brief	���x�����������B
		 *@param[in]	levelDataFilePath		tkl�t�@�C���̃t�@�C���p�X�B
		 *@param[in] hookFunc				�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g�B
		 *   �t�b�N���Ȃ��Ȃ�nullptr���w�肷��΂悢�A
		 * ���̊֐��I�u�W�F�N�g��false��Ԃ��ƁA�I�u�W�F�N�g�̏�񂩂�A
		 * �ÓI�I�u�W�F�N�g��MapChip�N���X�̃C���X�^���X����������܂��B
		 * �I�u�W�F�N�g�̖��O�ȂǂŁA�h�A��W�����v��A�A�C�e���Ȃǂ̓���ȃN���X�̃C���X�^���X�𐶐������Ƃ��ɁA
		 * �f�t�H���g�ō쐬�����MapChip�N���X�̃C���X�^���X���s�v�ȏꍇ��true��Ԃ��Ă��������B
		 * �Ⴆ�΁A�t�b�N�֐��̒��ŁA�n���ꂽ�I�u�W�F�N�g�f�[�^�̖��O�̃��f����`�悷��N���X�̃C���X�^���X��
		 * ���������Ƃ��ɁAfalse��Ԃ��Ă��܂��ƁA�������f������`�悳��邱�ƂɂȂ�܂��B
		 */
		void Init( const char* filePath,  std::function<bool(LevelObjectData& objData)> hookFunc);
	private:
		/// <summary>
		/// �}�b�v�`�b�v�����_���[���쐬�o������쐬����or�`�悷�ׂ��I�u�W�F�N�g�̐����C���N�������g����B
		/// </summary>
		/// <remarks>
		/// ���o�^�̃I�u�W�F�N�g���n���ꂽ�Ƃ��́A�����_���[���쐬���܂��B
		/// �o�^�ς݂̏ꍇ�́A�}�b�v�`�b�v�����_���[���`�悷�ׂ��I�u�W�F�N�g�̐���
		/// �C���N�������g����܂��B
		/// </remarks>
		/// <returns></returns>
		CMapChipRender* CreateMapChipRenderOrAddRenderObject( const LevelObjectData& objData );
	private:
		/// <summary>
		/// �{�[���s��̍\�z�B
		/// </summary>
		void BuildBoneMatrices();

		std::vector<CMapChipPtr> m_mapChipPtrs;			//�}�b�v�`�b�v�̉ϒ��z��B
		std::map< unsigned int, CMapChipRender*> m_mapChipRenderPtrs;	//�}�b�v�`�b�v�����_���[�̉ϒ��z��B

		CTklFile m_tklFile;
		static const int BONE_MAX = 512;				//�{�[���̍ő吔�B
		using CBonePtr = std::unique_ptr<CBone>;
		std::vector<CBonePtr>	m_bones;				//�{�[���̔z��B
		std::unique_ptr<CMatrix[]>	m_boneMatrixs;		//�{�[���s��B
		bool m_isInited = false;						//�������ς݁H
	};
}