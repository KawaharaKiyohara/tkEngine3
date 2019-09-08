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
	class  CTkmFile : Noncopyable {
	public:
		/// <summary>
		/// マテリアル
		/// </summary>
		struct SMaterial {
			std::string albedoMapFileName;		//アルベドマップのファイル名。
			std::string normalMapFileName;		//法線マップのファイル名。
			std::string specularMapFileName;	//スペキュラマップのファイル名。	
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
		/// インデックスバッファ。
		/// </summary>
		struct SIndexBuffer {
			std::vector< int > indices;	//インデックス。
		};
		/// <summary>
		/// メッシュパーツ。
		/// </summary>
		struct SMeshParts {
			std::vector< SMaterial > materials;			//マテリアルの配列。
			std::vector< SVertex >	vertexBuffer;		//頂点バッファ。
			std::vector<SIndexBuffer> indexBufferArray;	//インデックスバッファの配列。マテリアルの数分だけインデックスバッファはあるよ。
		};
		/// <summary>
		/// 3Dモデルを非同期ロード。
		/// </summary>
		/// <remarks>
		/// 本関数を使用した場合はメイン関数でIsLoaded関数を利用してロード終了待ちを行う必要があります。
		/// </remarks>
		/// <param name="filePath">ファイルパス。</param>
		void LoadAsync(const char* filePath);
		/// <summary>
		/// 3Dモデルをロード。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void Load(const char* filePath);
		/// <summary>
		/// 読み込みが終了しているか判定を行う。
		/// </summary>
		/// <returns>trueが返ってきたら読み込み終わり。</returns>
		bool IsLoaded() const
		{
			return m_isLoaded;
		}
	private:
		std::string LoadTextureFileName(FILE* fp);
	private:
		bool m_isLoaded = false;	//ロード済みフラグ。
		std::string m_filePath;		//ファイルパス。
		std::vector< SMeshParts>	m_meshParts;	//メッシュパーツ。
	};
}
