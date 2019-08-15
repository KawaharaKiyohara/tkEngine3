#pragma once

namespace tkEngine {
	class ITexture : Noncopyable {
	public:
		/// <summary>
		/// 利用可能？
		/// </summary>
		/// <returns>trueが返ってきたら利用可能。</returns>
		bool IsValid() const
		{
			return m_isValid;
		}
	protected:
		/// <summary>
		/// テクスチャを利用可能にする。
		/// </summary>
		void SetValid()
		{
			m_isValid = true;
		}
	private:
		bool m_isValid = false;	//利用可能？
	};
}