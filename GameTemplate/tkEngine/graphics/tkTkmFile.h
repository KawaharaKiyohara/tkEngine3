/// <summary>
/// tkm�t�@�C���B
/// </summary>
/// <remarks>
/// tkm�t�@�C����tkEngine�Ǝ���3D���f���t�H�[�}�b�g�ł��B
/// �{�N���X��tkm�t�@�C���̃��[�_�[�ł��B
/// </remarks>

#pragma once


namespace tkEngine {
	
	/// <summary>
	/// tkm�t�@�C���N���X�B
	/// </summary>
	class  CTkmFile : Noncopyable {
	public:
		/// <summary>
		/// �}�e���A��
		/// </summary>
		struct SMaterial {
			std::string albedoMapFileName;		//�A���x�h�}�b�v�̃t�@�C�����B
			std::string normalMapFileName;		//�@���}�b�v�̃t�@�C�����B
			std::string specularMapFileName;	//�X�y�L�����}�b�v�̃t�@�C�����B	
		};
		/// <summary>
		/// ���_�B
		/// </summary>
		struct SVertex {
			CVector3 pos;			//���W�B
			CVector3 normal;		//�@���B
			CVector2 uv;			//UV���W�B
			CVector4 skinWeights;	//�X�L���E�F�C�g�B
			short indices[4];		//�X�L���C���f�b�N�X�B
		};
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�B
		/// </summary>
		struct SIndexBuffer {
			std::vector< int > indices;	//�C���f�b�N�X�B
		};
		/// <summary>
		/// ���b�V���p�[�c�B
		/// </summary>
		struct SMeshParts {
			std::vector< SMaterial > materials;			//�}�e���A���̔z��B
			std::vector< SVertex >	vertexBuffer;		//���_�o�b�t�@�B
			std::vector<SIndexBuffer> indexBufferArray;	//�C���f�b�N�X�o�b�t�@�̔z��B�}�e���A���̐��������C���f�b�N�X�o�b�t�@�͂����B
		};
		/// <summary>
		/// 3D���f����񓯊����[�h�B
		/// </summary>
		/// <remarks>
		/// �{�֐����g�p�����ꍇ�̓��C���֐���IsLoaded�֐��𗘗p���ă��[�h�I���҂����s���K�v������܂��B
		/// </remarks>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void LoadAsync(const char* filePath);
		/// <summary>
		/// 3D���f�������[�h�B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void Load(const char* filePath);
		/// <summary>
		/// �ǂݍ��݂��I�����Ă��邩������s���B
		/// </summary>
		/// <returns>true���Ԃ��Ă�����ǂݍ��ݏI���B</returns>
		bool IsLoaded() const
		{
			return m_isLoaded;
		}
	private:
		std::string LoadTextureFileName(FILE* fp);
	private:
		bool m_isLoaded = false;	//���[�h�ς݃t���O�B
		std::string m_filePath;		//�t�@�C���p�X�B
		std::vector< SMeshParts>	m_meshParts;	//���b�V���p�[�c�B
	};
}
