/// <summary>
/// tklファイル
/// </summary>
/// <remarks>
/// tklファイルはtkEngine独自のレベルフォーマットです。
/// 本クラスはtklファイルのローダーです。
/// </remarks>
#pragma once

namespace tkEngine {
	class CTklFile : public IResource {
	public:
		/// <summary>
		/// オブジェクト。
		/// </summary>
		struct SObject {
			std::unique_ptr<char[]> name;	//モデルの名前。
			int parentNo;					//親の番号。
			float bindPose[4][3];			//バインドポーズ。
			float invBindPose[4][3];		//バインドポーズの逆数。
			int no;							//オブジェクトの番号。
			bool isShadowCaster; //シャドウキャスターフラグ。
			bool isShadowReceiver; //シャドウレシーバーフラグ。
			//この辺にライトの数、種類、座標などのデータも持たせるかも
			std::vector<int> intDatas; //intパラメータ。
			std::vector<float> floatDatas; //floatパラメータ。
			std::vector<std::unique_ptr<char[]>> charsDatas;	//文字列。
			std::vector<CVector3> vec3Datas; //vector3のパラメータ。
		};
		/// <summary>
		/// 読み込み処理。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadImplement(const char* filePath) override final;
		/// <summary>
		/// ボーンに対してクエリを行う。
		/// </summary>
		/// <param name="query">クエリ関数</param>
		void QueryObject(std::function<void(SObject& obj)> query)
		{
			for (auto& obj : m_objects) {
				query(obj);
			}
		}
	private:
		int m_tklVersion = 100; //tklファイルのバージョン
		int m_numObject = 0;			//オブジェクトの数。
		std::vector<SObject> m_objects;		//オブジェクトのリスト。
	};
}
