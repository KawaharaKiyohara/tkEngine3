#pragma once

namespace tkEngine {
	class CRootSignature : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="descriptorRange"></param>
		void Init(CD3DX12_DESCRIPTOR_RANGE1* descriptorRange);
	private:
		ComPtr<ID3D12RootSignature> m_rootSignature;	//ルートシグネチャ。
	};
}