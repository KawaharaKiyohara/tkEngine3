#pragma once

namespace tkEngine {
	/// <summary>
	/// ���\�[�X�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	class IResource : Noncopyable{
	public:
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		virtual ~IResource();
		/// <summary>
		/// �T�u�N���X�Ŏ�������ǂݍ��ݏ����̖{�́B
		/// </summary>
		virtual void LoadImplement(const char* filePath) = 0;
		/// <summary>
		/// �������[�h�B
		/// </summary>
		/// <param name="filePath"></param>
		void Load(const char* filePath)
		{
			m_filePath = filePath;
			LoadImplement(filePath);
		}
		/// <summary>
		/// �񓯊����[�h�B
		/// </summary>
		/// <param name="filePath"></param>
		void LoadAsync( const char* filePath );
		/// <summary>
		/// �ǂݍ��ݏI������B
		/// </summary>
		/// <returns></returns>
		bool IsLoaded() const
		{
			return m_isLoaded;
		}
	protected:
	
		/// <summary>
		/// ���[�h�ς݂ɂ���B
		/// </summary>
		void SetLoadedMark()
		{
			m_isLoaded = true;
		}
	private:
		std::string m_filePath;
		bool m_isLoaded = false;	//�ǂݍ��݊����t���O�B
		std::unique_ptr< std::thread > m_loadThread;
	};
}