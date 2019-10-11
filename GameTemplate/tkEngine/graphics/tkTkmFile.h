/// <summary>
/// tkmファイル。
/// </summary>
/// <remarks>
/// tkmファイルはtkEngine独自の3Dモデルフォーマットです。
/// 本クラスはtkmファイルのローダーです。
/// </remarks>

#pragma once


namespace tkEngine {
	
	/// <summary>
	/// tkmファイルクラス。
	/// </summary>
	class  CTkmFile : public IResource {
	public:
		/// <summary>
		/// マテリアル
		/// </summary>
		struct SMaterial {
			std::string albedoMapFileName;			//アルベドマップのファイル名。
			std::string normalMapFileName;			//法線マップのファイル名。
			std::string specularMapFileName;		//スペキュラマップのファイル名。
			std::unique_ptr<char[]>	albedoMap;		//ロードされたアルベドマップ。(ddsファイル)
			unsigned int albedoMapSize;				//アルベドマップのサイズ。(ddsファイル)
			std::unique_ptr<char[]>	normalMap;		//ロードされた法線マップ。(ddsファイル)
			unsigned int normalMapSize;				//法線マップのサイズ。
			std::unique_ptr<char[]>	specularMap;	//ロードされたスペキュラマップ。(ddsファイル)
			unsigned int specularMapSize;			//スペキュラマップのサイズ。(ddsファイル)
		};
		/// <summary>
		/// 頂点。
		/// </summary>
		struct SVertex {
			CVector3 pos;			//座標。
			CVector3 normal;		//法線。
			CVector2 uv;			//UV座標。
			CVector4 skinWeights;	//スキンウェイト。
			short indices[4];		//スキンインデックス。
		};
		/// <summary>
		/// 32ビットのインデックスバッファ。
		/// </summary>
		struct SIndexBuffer32 {
			std::vector< std::uint32_t > indices;	//インデックス。
		};
		/// <summary>
		/// 16ビットのインデックスバッファ。
		/// </summary>
		struct SIndexbuffer16 {
			std::vector< std::uint16_t > indices;	//インデックス。
		};
		/// <summary>
		/// メッシュパーツ。
		/// </summary>
		struct SMesh {
			std::vector< SMaterial > materials;				//マテリアルの配列。
			std::vector< SVertex >	vertexBuffer;			//頂点バッファ。
			std::vector<SIndexBuffer32> indexBuffer32Array;	//インデックスバッファの配列。マテリアルの数分だけインデックスバッファはあるよ。
			std::vector< SIndexbuffer16> indexBuffer16Array;
		};
		
		/// <summary>
		/// 3Dモデルをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void LoadImplement(const char* filePath) override final;
		
		/// <summary>
		/// メッシュパーツに対してクエリを行う。
		/// </summary>
		/// <param name="func">クエリ関数</param>
		void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
		{
			for (auto& mesh : m_meshParts) {
				func(mesh);
			}
		}
		/// <summary>
		/// メッシュの数を取得。
		/// </summary>
		/// <returns></returns>
		int GetNumMesh() const
		{
			return (int)(m_meshParts.size());
		}
	private:
		/// <summary>
		/// テクスチャ名をロード。
		/// </summary>
		/// <param name="fp"></param>
		/// <returns></returns>
		std::string LoadTextureFileName(FILE* fp);
		/// <summary>
		/// インデックスバッファをロード。
		/// </summary>
		template<class T>
		void LoadIndexBuffer(std::vector<T>& indexBuffer, int numIndex, FILE* fp);
		/// <summary>
		/// マテリアルを構築。
		/// </summary>
		/// <param name="tkmMat"></param>
		void BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath);
	private:
		std::vector< SMesh	>		m_meshParts;		//メッシュパーツ。
		std::unique_ptr< std::thread > m_loadThread;	//ロードスレッド。
	};
}
