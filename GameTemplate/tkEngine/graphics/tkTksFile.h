/// <summary>
/// tks�t�@�C��
/// </summary>
/// <remarks>
/// tks�t�@�C����tkEngine�Ǝ��̃X�P���g���t�H�[�}�b�g�ł��B
/// �{�N���X��tks�t�@�C���̃��[�_�[�ł��B
/// </remarks>
#pragma once

namespace tkEngine {
	class CTksFile : public IResource{
	public:
		/// <summary>
		/// �{�[���B
		/// </summary>
		struct SBone {
			std::unique_ptr<char[]> name;	//���̖��O�B
			std::int32_t parentId;			//�e��ID
			float bindPose[4][3];			//�o�C���h�|�[�Y�B
			float invBindPose[4][3];		//�o�C���h�|�[�Y�̋t���B

		};
		/// <summary>
		/// �ǂݍ��ݏ����B
		/// </summary>
		/// <param name="filePath"></param>
		void LoadImplement(const char* filePath) override final;
		/// <summary>
		/// �{�[���ɑ΂��ăN�G�����s���B
		/// </summary>
		/// <param name="query">�N�G���֐�</param>
		void QueryBone(std::function<void(SBone & bone)> query) 
		{
			for (auto& bone : m_bones) {
				query(bone);
			}
		}
	private:
		int m_numBone = 0;			//���̐��B
		std::vector<SBone> m_bones;	//���̃��X�g�B
	};
}
