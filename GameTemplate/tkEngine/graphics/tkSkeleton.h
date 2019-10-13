/*!
 *@brief	スケルトン
 */
#pragma once

#include "tkEngine/graphics/tkTksFile.h"

namespace tkEngine{
	class CAnimation;
	/*!
	*@brief	ボーン。
	*/
	class CBone : Noncopyable{
	public:
		/*!
		*@brief	コンストラクタ。
		*@param[in]	boneName		ボーンの名前。
		*@param[in]	bindPose		バインドポーズの行列。
		*@param[in] invBindPose		バインドポーズの逆行列。
		*@param[in]	parentId		親のボーン番号。
		*@param[in] boneId			ボーン番号。
		*/
		CBone(
			const wchar_t* boneName,
			const CMatrix& bindPose,
			const CMatrix& invBindPose,
			int parentId,
			int boneId
		) :
			m_boneName(boneName),
			m_bindPose(bindPose),
			m_invBindPose(invBindPose),
			m_parentId(parentId),
			m_worldMatrix(bindPose),
			m_boneId(boneId)
		{
		}
		/*!
		 *@brief	ローカル行列を設定
		 */
		void SetLocalMatrix(const CMatrix& m)
		{
			m_localMatrix = m;
		}
		/*!
		 *@brief	ローカル行列を取得。
		 */
		const CMatrix& GetLocalMatrix() const
		{
			return m_localMatrix;
		}
		/*!
		*@brief	ワールド行列を設定。
		*/
		void SetWorldMatrix(const CMatrix& m)
		{
			m_worldMatrix = m;
		}
		/*!
		 *@brief	ワールド行列を取得。
		 */
		const CMatrix& GetWorldMatrix() const
		{
			return m_worldMatrix;
		}
		/*!
		*@brief	バインドポーズの行列を取得。
		*/
		const CMatrix& GetBindPoseMatrix() const
		{
			return m_bindPose;
		}
		/*!
		 *@brief	バインドポーズの逆行列を取得。
		 */
		const CMatrix& GetInvBindPoseMatrix() const
		{
			return m_invBindPose;
		}
		/*!
		 *@brief	親のボーン行列を取得。
		 */
		int GetParentId() const
		{
			return m_parentId;
		}
		/*!
		 *@brief	ボーン番号の取得。
		 */
		int GetNo() const
		{
			return m_boneId;
		}

		/*!
		*@brief	子供を追加。
		*/
		void AddChild(CBone* bone) 
		{
			m_children.push_back(bone);
		}
		/*!
		*@brief	子供を取得。
		*/
		std::list<CBone*>& GetChildren()
		{
			return m_children;
		}
		const CMatrix& GetOffsetLocalMatrix() const
		{
			return m_offsetLocalMatrix;
		}
		/*!
		 *@brief	名前の取得。
		 */
		const wchar_t* GetName() const
		{
			return m_boneName.c_str();
		}
		/*!
		*@brief	このボーンのワールド空間での位置と回転とスケールを計算する。
		*@param[out]	trans		平行移動量の格納先。
		*@param[out]	rot			回転量の格納先。
		*@param[out]	scale		拡大率の格納先。
		*/
		void CalcWorldTRS(CVector3& trans, CQuaternion& rot, CVector3& scale);
		
	private:
	
		std::wstring	m_boneName;
		int				m_parentId = -1;	//!<親のボーン番号。
		int				m_boneId = -1;		//!<ボーン番号。
		CMatrix			m_bindPose;		//!<バインドポーズ。
		CMatrix			m_invBindPose;	//!<バインドポーズの逆行列。
		CMatrix			m_localMatrix = CMatrix::Identity;	//!<ローカル行列。
		CMatrix			m_worldMatrix = CMatrix::Identity;	//!<ワールド行列。
		CMatrix			m_offsetLocalMatrix;
		CVector3		m_positoin = CVector3::Zero;		//!<このボーンのワールド空間での位置。最後にCalcWorldTRSを実行したときの結果が格納されている。
		CVector3		m_scale = CVector3::One;			//!<このボーンの拡大率。最後にCalcWorldTRSを実行したときの結果が格納されている。
		CQuaternion		m_rotation = CQuaternion::Identity;	//!<このボーンの回転。最後にCalcWorldTRSを実行したときの結果が格納されている。
		std::list<CBone*>	m_children;		//!<子供。
	};

	/// <summary>
	/// スケルトンクラス。
	/// </summary>
	class CSkeleton : Noncopyable {
	public:
		using OnPostProcessSkeletonUpdate = std::function<void()>;

		CSkeleton();
		~CSkeleton();
		
		/// <summary>
		/// ボーンのローカル行列を設定。
		/// </summary>
		/// <param name="boneNo">ボーン番号</param>
		/// <param name="m">行列</param>
		void SetBoneLocalMatrix(int boneNo, const CMatrix& m)
		{
			if (boneNo > (int)m_bones.size() - 1) {
				TK_WARNING("boneNo is over m_bones.size() ");
				return;
			}
			m_bones[boneNo]->SetLocalMatrix(m);
		}

		/// <summary>
		/// ボーンの数を取得。
		/// </summary>
		/// <returns></returns>
		int GetNumBones() const
		{
			return static_cast<int>(m_bones.size());
		}
		/// <summary>
		/// 同期ロード。
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <returns>trueが返ってきたらロード成功。</returns>
		bool Load(const char* filePath);
		/// <summary>
		/// 非同期ロード。
		/// IsLoaded関数を使用して、同期をとるようにしてください。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void LoadAsync(const char* filePath)
		{
			m_tksFile.LoadAsync(filePath);
		}
		/// <summary>
		/// tksファイルのロードが完了したか判定。
		/// </summary>
		/// <returns></returns>
		bool IsLoaded() const 
		{
			return m_tksFile.IsLoaded();
		}
		/// <summary>
		/// ボーン行列の構築。
		/// 読み込みが完了した後で呼び出してください。
		/// </summary>
		void BuildBoneMatrices();
		/// <summary>
		/// ボーンの名前からボーン番号を検索。
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns>ボーン番号。見つからなかった場合は-1が返ってきます。</returns>
		int FindBoneID(const wchar_t* boneName) const
		{
			for (int i = 0; i < (int)m_bones.size(); i++) {
				if (wcscmp(m_bones[i]->GetName(), boneName) == 0) {
					return i;
				}
			}
			//見つからなかった。
			return -1;
		}
		/// <summary>
		/// ボーン番号からボーンを取得。
		/// </summary>
		/// <param name="boneNo">ボーン番号</param>
		/// <returns>ボーン</returns>
		CBone* GetBone(int boneNo) const
		{
			return m_bones[boneNo].get();
		}
	public:
		

		/// <summary>
		/// 更新。
		/// </summary>
		/// <param name="mWorld"></param>
		void Update(const CMatrix& mWorld);
	
		/// <summary>
		/// ボーンのワールド行列の更新関数。
		/// </summary>
		/// <remarks>
		/// 通常はユーザーがこの関数を呼び出す必要はありません。
		/// </remarks>
		/// <param name="bone">更新するボーン。</param>
		/// <param name="parentMatrix">親のボーンのワールド行列。</param>
		static 	void UpdateBoneWorldMatrix(CBone& bone, const CMatrix& parentMatrix);

	private:
		CTksFile m_tksFile;								//TKSファイル。
		static const int BONE_MAX = 512;				//ボーンの最大数。
		using CBonePtr = std::unique_ptr<CBone>;
		std::vector<CBonePtr>	m_bones;				//ボーンの配列。
		std::unique_ptr<CMatrix[]>	m_boneMatrixs;		//ボーン行列。
	};
}