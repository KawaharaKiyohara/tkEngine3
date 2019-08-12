#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkIPrimitive.h"

namespace tkEngine {
	/// <summary>
	/// Dx12版のプリミティブの実装クラス。
	/// </summary>
	class CPrimitiveDx12 : public IPrimitive {
	public:
		/// <summary>
		/// コンストラクタ。
		/// </summary>
		CPrimitiveDx12();
		/// <summary>
		/// 描画。
		/// </summary>
		void Draw(CRenderContext& rc) override final;
	private:
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12