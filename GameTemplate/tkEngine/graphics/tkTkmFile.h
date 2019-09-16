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
		/// 32�r�b�g�̃C���f�b�N�X�o�b�t�@�B
		/// </summary>
		struct SIndexBuffer32 {
			std::vector< std::uint32_t > indices;	//�C���f�b�N�X�B
		};
		/// <summary>
		/// 16�r�b�g�̃C���f�b�N�X�o�b�t�@�B
		/// </summary>
		struct SIndexbuffer16 {
			std::vector< std::uint16_t > indices;	//�C���f�b�N�X�B
		};
		/// <summary>
		/// ���b�V���p�[�c�B
		/// </summary>
		struct SMeshParts {
			std::vector< SMaterial > materials;				//�}�e���A���̔z��B
			std::vector< SVertex >	vertexBuffer;			//���_�o�b�t�@�B
			std::vector<SIndexBuffer32> indexBuffer32Array;	//�C���f�b�N�X�o�b�t�@�̔z��B�}�e���A���̐��������C���f�b�N�X�o�b�t�@�͂����B
			std::vector< SIndexbuffer16> indexBuffer16Array;
		};
		/// <summary>
		/// �f�X�g���N�^�B
		/// </summary>
		~CTkmFile();
		
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
		/// <summary>
		/// �e�N�X�`���������[�h�B
		/// </summary>
		/// <param name="fp"></param>
		/// <returns></returns>
		std::string LoadTextureFileName(FILE* fp);
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�����[�h�B
		/// </summary>
		template<class T>
		void LoadIndexBuffer(std::vector<T>& indexBuffer, int numIndex, FILE* fp);
	private:
		bool m_isLoaded = false;						//���[�h�ς݃t���O�B
		std::string m_filePath;							//�t�@�C���p�X�B
		std::vector< SMeshParts	>	m_meshParts;		//���b�V���p�[�c�B
		std::unique_ptr< std::thread > m_loadThread;	//���[�h�X���b�h�B
	};
}
