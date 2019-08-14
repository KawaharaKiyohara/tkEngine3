#pragma once

namespace tkEngine{
	/// <summary>
	/// 形状のインターフェースクラス。
	/// </summary>
	class IShape : Noncopyable{
	public:
		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="position">座標。</param>
		void SetPosition(const CVector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const CQuaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 拡大率を設定。
		/// </summary>
		/// <param name="scale">拡大率</param>
		void SetScale( const CVector3& scale )
		{
			m_scale = scale;
		}
		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns>座標</returns>
		const CVector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 回転を取得。
		/// </summary>
		/// <returns>回転</returns>
		const CQuaternion& GetRotation() const
		{
			return m_rotation;
		}
		/// <summary>
		/// 拡大率を取得。
		/// </summary>
		/// <returns>拡大率。</returns>
		const CVector3& GetScale()const
		{
			return m_scale;
		}
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc"></param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj) = 0;
	private:
		/// <summary>
		/// ワールド行列を更新。
		/// </summary>
		void UpdateWorldMatrix();
	protected:
		CVector3 m_position = CVector3::Zero;			//座標。
		CQuaternion m_rotation = CQuaternion::Identity;	//回転。
		CVector3 m_scale = CVector3::One;				//拡大率。
		CMatrix m_worldMatrix = CMatrix::Identity;		//ワールド行列。
	};
}