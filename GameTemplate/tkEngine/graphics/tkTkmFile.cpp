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
			std::int16_t indices[4];		//�X�L���C���f�b�N�X�B
		};
	};
	CTkmFile::~CTkmFile()
	{
		if (m_loadThread) {
			//�ǂݍ��݃X���b�h���I���܂őҋ@�B
			m_loadThread->join();
		}
	}
	std::string CTkmFile::LoadTextureFileName(FILE* fp)
	{
		std::string fileName;
		std::uint32_t fileNameLen;
		fread(&fileNameLen, sizeof(fileNameLen), 1, fp);
		if (fileNameLen > 0) {
			//��������L�^�ł���̈���X�^�b�N����m�ہB
			char* localFileName = reinterpret_cast<char*>(alloca(fileNameLen + 1));
			//�k�����������ǂݍ��ނ̂Ł{�P
			fread(localFileName, fileNameLen + 1, 1, fp);
			fileName = localFileName;
		}
		return fileName;
	}
	template<class T>
	void CTkmFile::LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp)
	{
		indices.resize(numIndex);
		for (int indexNo = 0; indexNo < numIndex; indexNo++) {
			T index;
			fread(&index, sizeof(index), 1, fp);
			indices[indexNo] = index - 1;	//todo max�̃C���f�b�N�X��1����J�n���Ă���̂ŁA-1����B
										//todo �G�N�X�|�[�^�[�Ō��炷�悤�ɂ��܂��傤�B
		}
	}

	void CTkmFile::BuildMaterial(SMaterial& tkmMat, FILE* fp)
	{
		//�A���x�h�̃t�@�C���������[�h�B
		tkmMat.albedoMapFileName = LoadTextureFileName(fp);
		//�@���}�b�v�̃t�@�C���������[�h�B
		tkmMat.normalMapFileName = LoadTextureFileName(fp);
		//�X�y�L�����}�b�v�̃t�@�C���������[�h�B
		tkmMat.specularMapFileName = LoadTextureFileName(fp);

		//����v���b�g�t�H�[���Ɉˑ�����ȁE�E�E�B�}���`�v���b�g�t�H�[���߂�ǂ����E�E�E�B
		std::string texFilePath = m_filePath;
		auto loadTexture = [&](
			std::string& texFileName, 
			std::unique_ptr<char[]>& ddsFileMemory, 
			unsigned int& fileSize
		) {
			int filePathLength = texFilePath.length();
			if (texFileName.length() > 0) {
				//���f���̃t�@�C���p�X���烉�X�g�̃t�H���_��؂��T���B
				auto replaseStartPos = texFilePath.find_last_of('/');
				if (replaseStartPos == std::string::npos) {
					replaseStartPos == texFilePath.find_last_of('\\');
				}
				replaseStartPos += 1;
				auto replaceLen = filePathLength - replaseStartPos;
				texFilePath.replace(replaseStartPos, replaceLen, texFileName);
				//�g���q��dds�ɕύX����B
				replaseStartPos = texFilePath.find_last_of('.') + 1;
				replaceLen = texFilePath.length() - replaseStartPos;
				texFilePath.replace(replaseStartPos, replaceLen, "dds");
				
				//�e�N�X�`�������[�h�B
				auto texFileFp = fopen(texFilePath.c_str(), "rb");
				if (texFileFp != nullptr) {
					//�t�@�C���T�C�Y���擾�B
					fseek(texFileFp, 0L, SEEK_END);		
					fileSize = ftell(texFileFp);
					fseek(texFileFp, 0L, SEEK_SET);

					ddsFileMemory = std::make_unique<char[]>(fileSize);
					fread(ddsFileMemory.get(), fileSize, 1, texFileFp);
					fclose(texFileFp);
				}
			}
		};
		//�e�N�X�`�������[�h�B
		loadTexture( tkmMat.albedoMapFileName, tkmMat.albedoMap, tkmMat.albedoMapSize );
		loadTexture( tkmMat.normalMapFileName, tkmMat.normalMap, tkmMat.normalMapSize );
		loadTexture( tkmMat.specularMapFileName, tkmMat.specularMap, tkmMat.specularMapSize );
	}
	void CTkmFile::LoadAsync(const char* filePath)
	{
		//�t�@�C���ǂݍ��݂͕ʃX���b�h�ōs���B
		m_filePath = filePath;
		m_loadThread = std::make_unique<std::thread>(
			[&]() {	Load(m_filePath.c_str());  } );

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
			//�}�e���A�������\�z���Ă����B
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				auto& material = meshParts.materials[materialNo];
				BuildMaterial(material, fp);
			}
			//�����Ē��_�o�b�t�@�B
			meshParts.vertexBuffer.resize(meshPartsHeader.numVertex);
			for (int vertNo = 0; vertNo < meshPartsHeader.numVertex; vertNo++) {
				tkmFileFormat::SVertex vertexTmp;
				fread(&vertexTmp, sizeof(vertexTmp), 1, fp);
				auto& vertex = meshParts.vertexBuffer[vertNo];
				vertex.pos.Set(vertexTmp.pos[0], vertexTmp.pos[1], vertexTmp.pos[2]);
				vertex.normal.Set(vertexTmp.normal[0], vertexTmp.normal[1], vertexTmp.normal[2]);
				vertex.uv.Set(vertexTmp.uv[0], vertexTmp.uv[1]);
				vertex.skinWeights.Set(vertexTmp.weights[0], vertexTmp.weights[1], vertexTmp.weights[2], vertexTmp.weights[3]);
				vertex.indices[0] = vertexTmp.indices[0];
				vertex.indices[1] = vertexTmp.indices[1];
				vertex.indices[2] = vertexTmp.indices[2];
				vertex.indices[3] = vertexTmp.indices[3];
			}
			//�����ăC���f�b�N�X�o�b�t�@�B
			//�C���f�b�N�X�o�b�t�@�̓}�e���A���̐����������݂���񂶂��B
			if (meshPartsHeader.indexSize == 2) {
				//16bit�̃C���f�b�N�X�o�b�t�@�B
				meshParts.indexBuffer16Array.resize(meshPartsHeader.numMaterial);
			}
			else {
				//32bit�̃C���f�b�N�X�o�b�t�@�B
				meshParts.indexBuffer32Array.resize(meshPartsHeader.numMaterial);
			}
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				//�|���S���������[�h�B
				int numPolygon;
				fread(&numPolygon, sizeof(numPolygon), 1, fp);
				//�g�|���W�[�̓g���C�A���O�����X�g�I�����[�Ȃ̂ŁA3����Z����ƃC���f�b�N�X�̐��ɂȂ�B
				int numIndex = numPolygon * 3;
				if (meshPartsHeader.indexSize == 2) {
					LoadIndexBuffer(
						meshParts.indexBuffer16Array[materialNo].indices,
						numIndex,
						fp 
					);
				}
				else {
					LoadIndexBuffer(
						meshParts.indexBuffer32Array[materialNo].indices,
						numIndex,
						fp
					);
				}

			}
		}

		fclose(fp);
		//�ǂݍ��ݏI���̈�B
		m_isLoaded = true;
	}
}
