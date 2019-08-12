#pragma once

namespace tkEngine {
	/// <summary>
	/// ���_�o�b�t�@�̃C���^�[�t�F�[�X�B
	/// </summary>
	class IVertexBuffer : Noncopyable {
	public:
		virtual~ IVertexBuffer() {}
		/// <summary>
		/// ���_�o�b�t�@�̍쐬�B
		/// </summary>
		/// <param name="size">���_�o�b�t�@�̃T�C�Y</param>
		/// <param name="stride">���_�̃T�C�Y�B</param>
		/// <returns></returns>
		static std::unique_ptr<IVertexBuffer> Create(int size, int stride);
		/// <summary>
		/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[�B
		/// </summary>
		/// <param name="srcVertices">�R�s�[���̒��_�f�[�^�B</param>
		virtual void Copy(void* srcVertices) = 0;
	private:
	};
}