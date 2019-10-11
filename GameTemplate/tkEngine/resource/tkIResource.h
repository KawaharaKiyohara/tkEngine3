#pragma once

namespace tkEngine {
	/// <summary>
	/// リソースのインターフェースクラス。
	/// </summary>
	class IResource : Noncopyable{
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		virtual ~IResource();
		/// <summary>
		/// サブクラスで実装する読み込み処理の本体。
		/// </summary>
		virtual void LoadImplement(const char* filePath) = 0;
		/// <summary>
		/// 同期ロード。
		/// </summary>
		/// <param name="filePath"></param>
		void Load(const char* filePath)
		{
			m_filePath = filePath;
			LoadImplement(filePath);
		}
		/// <summary>
		/// 非同期ロード。
		/// </summary>
		/// <param name="filePath"></param>
		void LoadAsync( const char* filePath );
		/// <summary>
		/// 読み込み終了判定。
		/// </summary>
		/// <returns></returns>
		bool IsLoaded() const
		{
			return m_isLoaded;
		}
	protected:
	
		/// <summary>
		/// ロード済みにする。
		/// </summary>
		void SetLoadedMark()
		{
			m_isLoaded = true;
		}
	private:
		std::string m_filePath;
		bool m_isLoaded = false;	//読み込み完了フラグ。
		std::unique_ptr< std::thread > m_loadThread;
	};
}