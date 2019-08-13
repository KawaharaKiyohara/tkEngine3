#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkRenderContext.h"

namespace tkEngine {
	/// <summary>
	/// �����_�����O�R���e�L�X�g�B
	/// </summary>
	class CRenderContextDx12 : public IRenderContext {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="commandList">�R�}���h���X�g�B</param>
		void Init(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
		/// <summary>
		/// �R�}���h���X�g���擾�B
		/// </summary>
		/// <returns>�R�}���h���X�g�B</returns>
		ComPtr<ID3D12GraphicsCommandList> GetCommandList()
		{
			return m_commandList;
		}
		/// <summary>
		/// �R�}���h���X�g��ݒ�B
		/// </summary>
		/// <param name="commandList">�R�}���h���X�g�B</param>
		void SetCommandList(ComPtr<ID3D12GraphicsCommandList> commandList)
		{
			m_commandList = commandList;
		}
	private:
		ComPtr<ID3D12GraphicsCommandList> m_commandList;	//�R�}���h���X�g�B
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

