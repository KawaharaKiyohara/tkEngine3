#pragma once

namespace tkEngine {
	/// <summary>
	/// パイプラインステート。
	/// </summary>
	class CPipelineStateDx12 : Noncopyable{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="desc"></param>
		void Init(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);
		ID3D12PipelineState* Get()
		{
			return m_pipelineState.Get();
		}
	private:
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
	};
}