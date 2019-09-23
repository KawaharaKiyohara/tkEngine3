#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkTkmFile.h"

namespace tkEngine {

	/// <summary>
	/// TKMファイルフォーマット。
	/// </summary>
	/// <remarks>
	/// 処理系によっては1バイトが8bitでないことがあり、
	/// int型やshort型が必ずしも、4バイト、2バイトであるとは限らない。
	/// そのため、std::uint16_tやstd::uint32_tを利用している。
	/// これらは定義されている処理系であれば、サイズは必ず同じである。
	/// </remarks>
	namespace tkmFileFormat {
		//現在のTKMファイルのバージョン。
		std::uint16_t VERSION = 100;
		/// <summary>
		/// ヘッダーファイル。
		/// </summary>

		struct SHeader {
			std::uint16_t	version;		//バージョン。
			std::uint16_t	numMeshParts;	//メッシュパーツの数。
		};
		/// <summary>
		/// メッシュパーツヘッダー。
		/// </summary>
		struct SMeshePartsHeader {
			std::uint32_t numMaterial;		//マテリアルの数。
			std::uint32_t numVertex;		//頂点数。
			std::uint8_t indexSize;			//インデックスのサイズ。2か4。
			std::uint8_t pad[3];			//パディング。
		};
		/// <summary>
		/// 頂点
		/// </summary>
		struct SVertex {
			float pos[3];					//頂点座標。
			float normal[3];				//法線。
			float uv[2];					//UV座標。
			float weights[4];				//スキンウェイト。
			std::int16_t indices[4];		//スキンインデックス。
		};
	};
	CTkmFile::~CTkmFile()
	{
		if (m_loadThread) {
			//読み込みスレッドが終わるまで待機。
			m_loadThread->join();
		}
	}
	std::string CTkmFile::LoadTextureFileName(FILE* fp)
	{
		std::string fileName;
		std::uint32_t fileNameLen;
		fread(&fileNameLen, sizeof(fileNameLen), 1, fp);
		if (fileNameLen > 0) {
			//文字列を記録できる領域をスタックから確保。
			char* localFileName = reinterpret_cast<char*>(alloca(fileNameLen + 1));
			//ヌル文字分も読み込むので＋１
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
			indices[indexNo] = index - 1;	//todo maxのインデックスは1から開始しているので、-1する。
										//todo エクスポーターで減らすようにしましょう。
		}
	}

	void CTkmFile::BuildMaterial(SMaterial& tkmMat, FILE* fp)
	{
		//アルベドのファイル名をロード。
		tkmMat.albedoMapFileName = LoadTextureFileName(fp);
		//法線マップのファイル名をロード。
		tkmMat.normalMapFileName = LoadTextureFileName(fp);
		//スペキュラマップのファイル名をロード。
		tkmMat.specularMapFileName = LoadTextureFileName(fp);

		//これプラットフォームに依存するな・・・。マルチプラットフォームめんどくさ・・・。
		std::string texFilePath = m_filePath;
		auto loadTexture = [&](
			std::string& texFileName, 
			std::unique_ptr<char[]>& ddsFileMemory, 
			unsigned int& fileSize
		) {
			int filePathLength = texFilePath.length();
			if (texFileName.length() > 0) {
				//モデルのファイルパスからラストのフォルダ区切りを探す。
				auto replaseStartPos = texFilePath.find_last_of('/');
				if (replaseStartPos == std::string::npos) {
					replaseStartPos == texFilePath.find_last_of('\\');
				}
				replaseStartPos += 1;
				auto replaceLen = filePathLength - replaseStartPos;
				texFilePath.replace(replaseStartPos, replaceLen, texFileName);
				//拡張子をddsに変更する。
				replaseStartPos = texFilePath.find_last_of('.') + 1;
				replaceLen = texFilePath.length() - replaseStartPos;
				texFilePath.replace(replaseStartPos, replaceLen, "dds");
				
				//テクスチャをロード。
				auto texFileFp = fopen(texFilePath.c_str(), "rb");
				if (texFileFp != nullptr) {
					//ファイルサイズを取得。
					fseek(texFileFp, 0L, SEEK_END);		
					fileSize = ftell(texFileFp);
					fseek(texFileFp, 0L, SEEK_SET);

					ddsFileMemory = std::make_unique<char[]>(fileSize);
					fread(ddsFileMemory.get(), fileSize, 1, texFileFp);
					fclose(texFileFp);
				}
			}
		};
		//テクスチャをロード。
		loadTexture( tkmMat.albedoMapFileName, tkmMat.albedoMap, tkmMat.albedoMapSize );
		loadTexture( tkmMat.normalMapFileName, tkmMat.normalMap, tkmMat.normalMapSize );
		loadTexture( tkmMat.specularMapFileName, tkmMat.specularMap, tkmMat.specularMapSize );
	}
	void CTkmFile::LoadAsync(const char* filePath)
	{
		//ファイル読み込みは別スレッドで行う。
		m_filePath = filePath;
		m_loadThread = std::make_unique<std::thread>(
			[&]() {	Load(m_filePath.c_str());  } );

	}
	void CTkmFile::Load(const char* filePath)
	{
		FILE* fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			TK_WARNING_MESSAGE_BOX("tkmファイルが開けません。ファイルパスが間違っていないか確認してください。%s", filePath);
			return ;
		}
		//tkmファイルのヘッダーを読み込み。
		tkmFileFormat::SHeader header;
		fread(&header, sizeof(header), 1, fp);
		if (header.version != tkmFileFormat::VERSION) {
			//tkmファイルのバージョンが違う。
			TK_WARNING_MESSAGE_BOX("tkmファイルのバージョンが異なっています。");
		}
		//メッシュ情報をロードしていく。
		m_meshParts.resize(header.numMeshParts);
		for (int meshPartsNo = 0; meshPartsNo < header.numMeshParts; meshPartsNo++) {
			auto& meshParts = m_meshParts[meshPartsNo];
			tkmFileFormat::SMeshePartsHeader meshPartsHeader;
			fread(&meshPartsHeader, sizeof(meshPartsHeader), 1, fp);
			//マテリアル情報を記録できる領域を確保。
			meshParts.materials.resize(meshPartsHeader.numMaterial);
			//マテリアル情報を構築していく。
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				auto& material = meshParts.materials[materialNo];
				BuildMaterial(material, fp);
			}
			//続いて頂点バッファ。
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
			//続いてインデックスバッファ。
			//インデックスバッファはマテリアルの数分だけ存在するんじゃよ。
			if (meshPartsHeader.indexSize == 2) {
				//16bitのインデックスバッファ。
				meshParts.indexBuffer16Array.resize(meshPartsHeader.numMaterial);
			}
			else {
				//32bitのインデックスバッファ。
				meshParts.indexBuffer32Array.resize(meshPartsHeader.numMaterial);
			}
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				//ポリゴン数をロード。
				int numPolygon;
				fread(&numPolygon, sizeof(numPolygon), 1, fp);
				//トポロジーはトライアングルリストオンリーなので、3を乗算するとインデックスの数になる。
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
		//読み込み終わりの印。
		m_isLoaded = true;
	}
}
