/// <summary>
/// tkl�t�@�C��
/// </summary>
/// <remarks>
/// tkl�t�@�C����tkEngine�Ǝ��̃��x���t�H�[�}�b�g�ł��B
/// �{�N���X��tkl�t�@�C���̃��[�_�[�ł��B
/// </remarks>
#pragma once

namespace tkEngine {
	class CTklFile : public IResource {
	public:
		/// <summary>
		/// �I�u�W�F�N�g�B
		/// </summary>
		struct SObject {
			std::unique_ptr<char[]> name;	//���f���̖��O�B
			int parentNo;					//�e�̔ԍ��B
			float bindPose[4][3];			//�o�C���h�|�[�Y�B
			float invBindPose[4][3];		//�o�C���h�|�[�Y�̋t���B
			int no;							//�I�u�W�F�N�g�̔ԍ��B
			bool isShadowCaster; //�V���h�E�L���X�^�[�t���O�B
			bool isShadowReceiver; //�V���h�E���V�[�o�[�t���O�B
			//���̕ӂɃ��C�g�̐��A��ށA���W�Ȃǂ̃f�[�^���������邩��
			std::vector<int> intDatas; //int�p�����[�^�B
			std::vector<float> floatDatas; //float�p�����[�^�B
			std::vector<std::unique_ptr<char[]>> charsDatas;	//������B
			std::vector<CVector3> vec3Datas; //vector3�̃p�����[�^�B
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
		void QueryObject(std::function<void(SObject& obj)> query)
		{
			for (auto& obj : m_objects) {
				query(obj);
			}
		}
	private:
		int m_tklVersion = 100; //tkl�t�@�C���̃o�[�W����
		int m_numObject = 0;			//�I�u�W�F�N�g�̐��B
		std::vector<SObject> m_objects;		//�I�u�W�F�N�g�̃��X�g�B
	};
}
