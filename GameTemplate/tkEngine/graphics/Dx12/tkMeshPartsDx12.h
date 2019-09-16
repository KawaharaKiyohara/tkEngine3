#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


#include "tkEngine/graphics/tkMeshParts.h"
#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#include <vector>
#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"
#include "tkEngine/graphics/Dx12/tkTextureDx12.h"
#include "tkEngine/graphics/Dx12/tkMaterialDx12.h"

namespace tkEngine {
	using UPCIndexBufferDx12 = std::unique_ptr<CIndexBufferDx12>;
	/// <summary>
	/// ���b�V��
	/// </summary>
	struct SMesh {
		CVertexBufferDx12 m_vertexBuffer;						//���_�o�b�t�@�B
		std::vector< UPCIndexBufferDx12 > m_indexBufferArray;	//�C���f�b�N�X�o�b�t�@�B
		std::vector< UPCMaterialDx12 >	m_materials;				//�}�e���A���B
	};
	using UPSMesh = std::unique_ptr<SMesh>;
	/// <summary>
	/// ���b�V���p�[�c�B
	/// </summary>
	class CMeshPartsDx12 : public IMeshParts {
	public:
		/// <summary>
		/// tkm�t�@�C�����珉�����B
		/// </summary>
		/// <param name="tkmFile"></param>
		void InitFromTkmFile(const CTkmFile& tkmFile) override final;
		
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// tkm���b�V�����烁�b�V�����쐬�B
		/// </summary>
		/// <param name="mesh">���b�V��</param>
		/// <param name="meshNo">���b�V���ԍ�</param>
		void CreateMeshFromTkmMesh(const CTkmFile::SMesh& mesh, int meshNo);
	private:
		std::vector< UPSMesh > m_meshs;		//���b�V���B
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
