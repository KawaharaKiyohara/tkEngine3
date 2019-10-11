/// <summary>
/// tksファイル
/// </summary>
/// <remarks>
/// tksファイルはtkEngine独自のスケルトンフォーマットです。
/// 本クラスはtksファイルのローダーです。
/// </remarks>
#pragma once

namespace tkEngine {
	class CTksFile : public IResource{
	public:
		/// <summary>
		/// ボーン。
		/// </summary>
		struct SBone {
			std::unique_ptr<char[]> name;	//骨の名前。
			std::int32_t parentId;			//親のID
			float bindPose[4][3];			//バインドポーズ。
			float invBindPose[4][3];		//バインドポーズの逆数。

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
		void QueryBone(std::function<void(SBone & bone)> query) 
		{
			for (auto& bone : m_bones) {
				query(bone);
			}
		}
	private:
		int m_numBone = 0;			//骨の数。
		std::vector<SBone> m_bones;	//骨のリスト。
	};
}
