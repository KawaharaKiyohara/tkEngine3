#pragma once

namespace tkEngine {
	/// <summary>
	/// 指向性シャドウマップの基底クラス
	/// </summary>
	/// <remarks>
	/// カスケードシャドウマップを採用。
	/// </remarks>
	class CDirectionalShadowMap : Noncopyable {
	public:
		/// <summary>
		/// シャドウマップへのレンダリング開始時に呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnBeginRenderToShadowMap(IRenderContext& rc) = 0;
	private:

	};
}