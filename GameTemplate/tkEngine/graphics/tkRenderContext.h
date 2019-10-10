#pragma once

namespace tkEngine {
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
	};
	using UPIRenderContext = std::unique_ptr<IRenderContext>;
}
