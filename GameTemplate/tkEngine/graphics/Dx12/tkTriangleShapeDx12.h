#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/Dx12/tkPrimitiveDx12.h"

namespace tkEngine {
	/// <summary>
	/// �O�p�`�̌`��N���X�B
	/// </summary>
	class CTriangleShapeDx12
	{
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CTriangleShapeDx12();

	private:
		CPrimitive m_primitive;
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};

	using CTriangleShape = CTriangleShapeDx12;
}

#endif