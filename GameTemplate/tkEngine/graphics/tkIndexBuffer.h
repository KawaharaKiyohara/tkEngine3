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
		/// �����������B
		/// </summary>
		/// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y�B</param>
		/// <param name="stride">�C���f�b�N�X�o�b�t�@�̃X�g���C�h�B</param>
		void Init(int size, int stride)
		{
			m_count = size / stride;
			InitSub( size, stride );
		}
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
		/// <summary>
		/// �T�u�N���X�Œ�`���鏉�����֐��B
		/// </summary>
		/// <remarks>
		/// �O���t�B�b�N�XAPI�Ɉˑ����鏉���������͂��̊֐����I�[�o�[���C�h���Ď������Ă��������B
		/// </remarks>
		/// <param name="size"></param>
		/// <param name="stride"></param>
		virtual void InitSub(int size, int stride) = 0;
	private:
		int m_count = 0;		//�C���f�b�N�X�̐��B
	};
}