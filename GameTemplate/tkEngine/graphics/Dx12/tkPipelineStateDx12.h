#pragma once

namespace tkEngine {
	/// <summary>
	/// �p�C�v���C���X�e�[�g�B
	/// </summary>
	class CPipelineStateDx12 : Noncopyable{
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="desc"></param>
		void Init(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);
		ID3D12PipelineState* Get()
		{
			return m_pipelineState.Get();
		}
	private:
		ComPtr<ID3D12PipelineState> m_pipelineState;	//�p�C�v���C���X�e�[�g�B
	};
}