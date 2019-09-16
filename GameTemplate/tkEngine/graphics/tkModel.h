#pragma once

#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/tkMeshParts.h"

namespace tkEngine {
	/// <summary>
	/// ���f���N���X�B
	/// </summary>
	class CModel : Noncopyable {
	public:
		/// <summary>
		/// tkm�t�@�C�������\�h�B
		/// </summary>
		/// <remarks>
		/// IsInited�֐��𗘗p���āA�������Ƃ�悤�ɂ��Ă��������B
		/// </remarks>
		/// <param name="filePath">tkm�t�@�C���̃t�@�C���p�X�B</param>
		void LoadTkmFileAsync(const char* filePath);
		/// <summary>
		/// ���b�V���p�[�c���쐬����B
		/// </summary>
		/// <remarks>
		/// ���̊֐���LoadTkmFileAsync�֐��Ȃǂ𗘗p���āA
		/// tkm�t�@�C�������[�h������ŌĂяo���Ă��������B
		/// </remarks>
		void CreateMeshParts();
		/// <summary>
		/// ���������I����Ă��邩����B
		/// </summary>
		/// <returns></returns>
		bool IsInited() const;
	private:
		CTkmFile m_tkmFile;			//tkm�t�@�C���B
		UPIMeshParts m_meshParts;	//���b�V���p�[�c�B
	};
}