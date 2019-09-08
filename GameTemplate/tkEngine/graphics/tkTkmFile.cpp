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
			std::int16_t indices[2];		//スキンインデックス。
		};
	};
	std::string CTkmFile::LoadTextureFileName(FILE* fp)
	{
		std::string fileName;
		std::uint32_t fileNameLen;
		fread(&fileNameLen, sizeof(fileNameLen), 1, fp);
		if (fileNameLen > 0) {
			//文字列を記録できる領域をスタックから確保。
			char* localFileName = reinterpret_cast<char*>(alloca(fileNameLen + 1));
			fread(localFileName, fileNameLen, 1, fp);
			//最後にヌル文字を入れる。
			localFileName[fileNameLen] = '\0';
			fileName = localFileName;
		}
		return fileName;
	}
	void CTkmFile::LoadAsync(const char* filePath)
	{
		//ファイル読み込みは別スレッドで行う。
		m_filePath = filePath;
		std::thread th([&]() {	Load(m_filePath.c_str());  });
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
			//マテリアル情報をロードしていく。
			for (int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
				auto& material = meshParts.materials[materialNo];
				//アルベドのファイル名をロード。
				material.albedoMapFileName = LoadTextureFileName(fp);
				//法線マップのファイル名をロード。
				material.normalMapFileName = LoadTextureFileName(fp);
				//スペキュラマップのファイル名をロード。
				material.specularMapFileName = LoadTextureFileName(fp);
			}
		}
	}
}
