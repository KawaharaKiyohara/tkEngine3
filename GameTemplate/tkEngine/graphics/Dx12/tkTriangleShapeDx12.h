#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"
#include "tkEngine/graphics/tkShape.h"

namespace tkEngine {
	/// <summary>
	/// �O�p�`�̌`��N���X�B
	/// </summary>
	class CTriangleShapeDx12 : public IShape
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CTriangleShapeDx12();
	
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		/// <param name="mView">�r���[�s��</param>
		/// <param name="mProj">�v���W�F�N�V�����s��B</param>
		void Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// �萔�o�b�t�@�B
		/// </summary>
		/// <remarks>
		/// ���̍\���̂�ύX������APrimitive.fx��CB���ύX����悤�ɁB
		/// </remarks>
		struct SConstantBuffer {
			CMatrix mWorld;		//���[���h�s��B
			CMatrix mView;		//�r���[�s��B
			CMatrix mProj;		//�v���W�F�N�V�����s��B
		};
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
		CShaderDx12 m_vs;								//���_�V�F�[�_�[�B
		CShaderDx12 m_ps;								//�s�N�Z���V�F�[�_�[�B
		CConstantBufferDx12 m_constantBuffer;			//�萔�o�b�t�@�B
	};

}

#endif