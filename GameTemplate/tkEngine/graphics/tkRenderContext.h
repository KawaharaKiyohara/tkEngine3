#pragma once

namespace tkEngine {
	/// <summary>
	/// レンダリングパス。
	/// </summary>
	enum EnRenderStep {
		enRenderStep_Undef,							//未定義
		enRenderStep_CreateDirectionalShadowMap,	//指向性シャドウマップの作成。
		enRenderStep_ForwardRender,					//フォワードレンダリング。
	};
	class IRenderContext : Noncopyable {
	public:
		
		virtual ~IRenderContext() {}
		/// <summary>
		/// IRenderContextのインスタンスの型変換。
		/// </summary>
		template<class T>
		T& As()
		{
			return dynamic_cast<T&>(*this);
		}
		/// <summary>
		/// レンダリングステップを設定。
		/// </summary>
		/// <param name="renderStep"></param>
		void SetRenderStep(EnRenderStep renderStep)
		{
			m_renderStep = renderStep;
		}
		/// <summary>
		/// 現在のレンダリングステップを取得。
		/// </summary>
		/// <returns></returns>
		EnRenderStep GetRenderStep() const
		{
			return m_renderStep;
		}
	private:
		EnRenderStep m_renderStep = enRenderStep_Undef;
	};
	using UPIRenderContext = std::unique_ptr<IRenderContext>;
}
