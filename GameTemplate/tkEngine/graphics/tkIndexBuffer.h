#pragma once

namespace tkEngine {
	class IModelImp;
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̃C���^�[�t�F�[�X�N���X�B
	/// </summary>
	class IIndexBuffer : Noncopyable {
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
		static std::unique_ptr<IIndexBuffer> Create( int size, int stride );
		/// <summary>
		/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
		virtual void Copy(void* srcIndecies) = 0;
	private:
	};
}