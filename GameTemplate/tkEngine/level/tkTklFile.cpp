#include "tkEngine/tkEnginePreCompile.h"
#include "tkTklFile.h"

namespace tkEngine {
	void CTklFile::LoadImplement(const char* filePath)
	{
		auto fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			return;
		}
		//�I�u�W�F�N�g�̐����擾�B
		fread(&m_numObject, sizeof(m_numObject), 1, fp);
		m_objects.resize(m_numObject);
		for (int i = 0; i < m_numObject; i++) {
			auto& obj = m_objects.at(i);
			size_t nameCount = 0;
			//�I�u�W�F�N�g�̖��O���擾�B
			fread(&nameCount, 1, 1, fp);
			obj.name = std::make_unique<char[]>(nameCount + 1);
			fread(obj.name.get(), nameCount + 1, 1, fp);
			//�e��ID���擾�B
			fread(&obj.parentNo, sizeof(obj.parentNo), 1, fp);
			//�o�C���h�|�[�Y���擾�B
			fread(obj.bindPose, sizeof(obj.bindPose), 1, fp);
			//�o�C���h�|�[�Y�̋t�����擾�B
			fread(obj.invBindPose, sizeof(obj.invBindPose), 1, fp);
			//�I�u�W�F�N�g�̔ԍ��B
			obj.no = i;
			//�V���h�E�L���X�^�[�̃t���O�B
			fread(&obj.isShadowCaster, sizeof(obj.isShadowCaster), 1, fp);
			//�V���h�E���V�[�o�[�̃t���O�B
			fread(&obj.isShadowReceiver, sizeof(obj.isShadowReceiver), 1, fp);
		}

		fclose(fp);
		//�ǂݍ��݊����̈�B
		SetLoadedMark();
	}
}