#pragma once

namespace tkEngine {
	class IModelImp;
	class IIndexBuffer;
	/// <summary>
	/// IIndexBuffer�̃��j�[�N�|�C���^�^�̕ʖ���`�B
	/// </summary>
	using UPIIndexBuffer = std	::unique_ptr<IIndexBuffer>;

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	class IIndexBuffer : public Noncopyable{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		virtual ~IIndexBuffer() {}
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@���쐬�B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y</param>
		/// <returns>�쐬���ꂽ�C���f�b�N�X�o�b�t�@�B</returns>
		static UPIIndexBuffer Create( int size, int stride );
		/// <summary>
		/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
		virtual void Copy(void* srcIndecies) = 0;
		/// <summary>
		/// �^�ϊ��B
		/// </summary>
		template<class T>
		T* As()
		{
			return dynamic_cast<T*>(this);
		}
		/// <summary>
		/// �C���f�b�N�X�̐����擾�B
		/// </summary>
		/// <returns>�C���f�b�N�X�̐��B</returns>
		int GetCount() const
		{
			return m_count;
		}
	private:
		int m_count = 0;		//�C���f�b�N�X�̐��B
	};
}