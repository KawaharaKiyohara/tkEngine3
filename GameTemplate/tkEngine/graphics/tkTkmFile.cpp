#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkTkmFile.h"

namespace tkEngine {
		
	/// <summary>
	/// TKM�t�@�C���t�H�[�}�b�g�B
	/// </summary>
	/// <remarks>
	/// �����n�ɂ���Ă�1�o�C�g��8bit�łȂ����Ƃ�����A
	/// int�^��short�^���K�������A4�o�C�g�A2�o�C�g�ł���Ƃ͌���Ȃ��B
	/// ���̂��߁Astd::uint16_t��std::uint32_t�𗘗p���Ă���B
	/// �����͒�`����Ă��鏈���n�ł���΁A�T�C�Y�͕K�������ł���B
	/// </remarks>
	namespace tkmFileFormat {
		//���݂�TKM�t�@�C���̃o�[�W�����B
		std::uint16_t VERSION = 100;
		/// <summary>
		/// �w�b�_�[�t�@�C���B
		/// </summary>

		struct SHeader {
			std::uint16_t	version;		//�o�[�W�����B
			std::uint16_t	numMeshParts;	//���b�V���p�[�c�̐��B
		};
		/// <summary>
		/// ���b�V���p�[�c�w�b�_�[�B
		/// </summary>
		struct SMeshePartsHeader {
			std::uint32_t numMaterial;		//�}�e���A���̐��B
			std::uint32_t numVertex;		//���_���B
			std::uint8_t indexSize;			//�C���f�b�N�X�̃T�C�Y�B2��4�B
			std::uint8_t pad[3];			//�p�f�B���O�B
		};
		/// <summary>
		/// ���_
		/// </summary>
		struct SVertex {
			float pos[3];					//���_���W�B
			float normal[3];				//�@���B
			float uv[2];					//UV���W�B
			float weights[4];				//�X�L���E�F�C�g�B
			std::int16_t indices[2];		//�X�L���C���f�b�N�X�B
		};
	};
	std::string CTkmFile::LoadTextureFileName(FILE* fp)
	{
		std::string fileName;
		std::uint32_t fileNameLen;
		fread(&fileNameLen, sizeof(fileNameLen), 1, fp);
		if (fileNameLen > 0) {
			//��������L�^�ł���̈���X�^�b�N����m�ہB
			char* localFileName = reinterpret_cast<char*>(alloca(fileNameLen + 1));
			fread(localFileName, fileNameLen, 1, fp);
			//�Ō�Ƀk������������B
			localFileName[fileNameLen] = '\0';
			fileName = localFileName;
		}
		return fileName;
	}
	void CTkmFile::LoadAsync(const char* filePath)
	{
		//�t�@�C���ǂݍ��݂͕ʃX���b�h�ōs���B
		m_filePath = filePath;
		std::thread th([&]() {	Load(m_filePath.c_str());  });
	}
	void CTkmFile::Load(const char* filePath)
	{
		FILE* fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			TK_WARNING_MESSAGE_BOX("tkm�t�@�C�����J���܂���B�t�@�C���p�X���Ԉ���Ă��Ȃ����m�F���Ă��������B%s", filePath);
			return ;
		}
		//tkm�t�@�C���̃w�b�_�[��ǂݍ��݁B
		tkmFileFormat::SHeader header;
		fread(&header, sizeof(header), 1, fp);
		if (header.version != tkmFileFormat::VERSION) {
			//tkm�t�@�C���̃o�[�W�������Ⴄ�B
			TK_WARNING_MESSAGE_BOX("tkm�t�@�C���̃o�[�W�������قȂ��Ă��܂��B");
		}
		//���b�V���������[�h���Ă����B
		m_meshParts.resize(header.numMeshParts);
		for (int meshPartsNo = 0; meshPartsNo < header.numMeshParts; meshPartsNo++) {
			auto& meshParts = m_meshParts[meshPartsNo];
			tkmFileFormat::SMeshePartsHeader meshPartsHeader;
			fread(&meshPartsHeader, sizeof(meshPartsHeader), 1, fp);
			//�}�e���A�������L�^�ł���̈���m�ہB
			meshParts.materials.resize(meshPartsHeader.numMaterial);
			//�}�e���A���������[�h���Ă����B
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				auto& material = meshParts.materials[materialNo];
				//�A���x�h�̃t�@�C���������[�h�B
				material.albedoMapFileName = LoadTextureFileName(fp);
				//�@���}�b�v�̃t�@�C���������[�h�B
				material.normalMapFileName = LoadTextureFileName(fp);
				//�X�y�L�����}�b�v�̃t�@�C���������[�h�B
				material.specularMapFileName = LoadTextureFileName(fp);
			}
		}
	}
}
