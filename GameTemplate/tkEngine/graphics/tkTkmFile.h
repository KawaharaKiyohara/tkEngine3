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
			string albedoMapFileName;			//アルベドマップのファイル名。
			string normalMapFileName;			//法線マップのファイル名。
			string specularMapFileName;		//スペキュラマップのファイル名。
			unique_ptr<char[]>	albedoMap;		//ロードされたアルベドマップ。(ddsファイル)
			unsigned int albedoMapSize;				//アルベドマップのサイズ。(ddsファイル)
			unique_ptr<char[]>	normalMap;		//ロードされた法線マップ。(ddsファイル)
			unsigned int normalMapSize;				//法線マップのサイズ。
			unique_ptr<char[]>	specularMap;	//ロードされたスペキュラマップ。(ddsファイル)
			unsigned int specularMapSize;			//スペキュラマップのサイズ。(ddsファイル)
		};
		/// <summary>
		/// 頂点。
		/// </summary>
		struct SVertex {
			CVector3 pos;			//座標。
			CVector3 normal;		//法線。
			CVector3 tangent;		//接ベクトル。
			CVector3 binormal;		//従ベクトル。
			CVector2 uv;			//UV座標。
			int indices[4];			//スキンインデックス。
			CVector4 skinWeights;	//スキンウェイト。
		};
		/// <summary>
		/// 32ビットのインデックスバッファ。
		/// </summary>
		struct SIndexBuffer32 {
			vector< uint32_t > indices;	//インデックス。
		};
		/// <summary>
		/// 16ビットのインデックスバッファ。
		/// </summary>
		struct SIndexbuffer16 {
			vector< uint16_t > indices;	//インデックス。
		};
		/// <summary>
		/// メッシュパーツ。
		/// </summary>
		struct SMesh {
			bool isFlatShading;							//フラットシェーディング？
			vector< SMaterial > materials;				//マテリアルの配列。
			vector< SVertex >	vertexBuffer;			//頂点バッファ。
			vector<SIndexBuffer32> indexBuffer32Array;	//インデックスバッファの配列。マテリアルの数分だけインデックスバッファはあるよ。
			vector< SIndexbuffer16> indexBuffer16Array;
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
		string LoadTextureFileName(FILE* fp);
		/// <summary>
		/// インデックスバッファをロード。
		/// </summary>
		template<class T>
		void LoadIndexBuffer(vector<T>& indexBuffer, int numIndex, FILE* fp);
		/// <summary>
		/// マテリアルを構築。
		/// </summary>
		/// <param name="tkmMat"></param>
		void BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath);
		/// <summary>
		/// 接ベクトルと従ベクトルを計算する。
		/// </summary>
		/// <remarks>
		/// 3dsMaxScriptでやるべきなんだろうけど、デバッグしたいので今はこちらでやる。
		/// </remarks>
		void BuildTangentAndBiNormal();
	private:
		vector< SMesh>	m_meshParts;		//メッシュパーツ。
	};
}
