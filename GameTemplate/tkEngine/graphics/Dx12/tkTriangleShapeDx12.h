#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"

namespace tkEngine {
	/// <summary>
	/// �O�p�`�̌`��N���X�B
	/// </summary>
	class CTriangleShapeDx12
	{
	public:

		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void Draw(IRenderContext& rc);
	private:
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
		CShaderDx12 m_vs;								//���_�V�F�[�_�[�B
		CShaderDx12 m_ps;								//�s�N�Z���V�F�[�_�[�B
	};

	using CTriangleShape = CTriangleShapeDx12;
}

#endif