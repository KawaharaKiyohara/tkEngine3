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
		/// <param name="mWorld">���[���h�s��</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mWorld, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// tkm���b�V�����烁�b�V�����쐬�B
		/// </summary>
		/// <param name="mesh">���b�V��</param>
		/// <param name="meshNo">���b�V���ԍ�</param>
		void CreateMeshFromTkmMesh(const CTkmFile::SMesh& mesh, int meshNo);
		/// <summary>
		/// ���ʒ萔�o�b�t�@�̍쐬�B
		/// </summary>
		void CreateCommonConstantBuffer();
		/// <summary>
		/// �f�B�X�N���v�^�q�[�v���쐬�B
		/// </summary>
		void CreateDescriptorHeaps();
	private:
		/// <summary>
		/// �萔�o�b�t�@�B
		/// </summary>
		/// <remarks>
		/// ���̍\���̂�ύX������ASimpleModel.fx��CB���ύX����悤�ɁB
		/// </remarks>
		struct SConstantBuffer {
			CMatrix mWorld;		//���[���h�s��B
			CMatrix mView;		//�r���[�s��B
			CMatrix mProj;		//�v���W�F�N�V�����s��B
		};
		CConstantBufferDx12 m_commonConstantBuffer;		//���b�V�����ʂ̒萔�o�b�t�@�B
		std::vector< UPSMesh > m_meshs;					//���b�V���B
		std::vector<ComPtr< ID3D12DescriptorHeap>>	m_descriptorHeaps;
		int m_cbrSrvDescriptorSize = 0;	
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
