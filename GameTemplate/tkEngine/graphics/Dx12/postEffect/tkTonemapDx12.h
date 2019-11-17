#pragma once

namespace tkEngine {
	/// <summary>
	/// トーンマップ。
	/// </summary>
	class CTonemapDx12 : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">グラフィック設定</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render(IRenderContext& rc);
	};
}