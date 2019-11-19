#pragma once

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkEngine/graphics/Dx12/tkRenderContextDx12.h"
#include "tkBloomDx12.h"

namespace tkEngine {
	/// <summary>
	/// ポストエフェクトクラス。
	/// </summary>
	class CPostEffectDx12 : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">グラフィック設定</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc"></param>
		void Render(IRenderContext& rc);
	
	private:
		CBloomDx12 m_bloom;	//ブルーム。
		CPrimitive m_quadPrimitive;			//四角形プリミティブ。

	};
}