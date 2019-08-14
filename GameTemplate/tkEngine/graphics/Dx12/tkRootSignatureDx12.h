#pragma once

namespace tkEngine {
	class CRootSignature : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="descriptorRange"></param>
		void Init(CD3DX12_DESCRIPTOR_RANGE1* descriptorRange);
	private:
		ComPtr<ID3D12RootSignature> m_rootSignature;	//���[�g�V�O�l�`���B
	};
}