#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkIPrimitive.h"

namespace tkEngine {
	/// <summary>
	/// Dx12�ł̃v���~�e�B�u�̎����N���X�B
	/// </summary>
	class CPrimitiveDx12 : public IPrimitive {
	public:
		/// <summary>
		/// �R���X�g���N�^�B
		/// </summary>
		CPrimitiveDx12();
		/// <summary>
		/// �`��B
		/// </summary>
		void Draw(CRenderContext& rc) override final;
	private:
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12