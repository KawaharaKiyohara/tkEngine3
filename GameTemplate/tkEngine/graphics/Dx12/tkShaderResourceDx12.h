#pragma once

namespace tkEngine {
	/// <summary>
	/// シェーダーリソースのインターフェースクラス。
	/// </summary>
	class  IShaderResourceDx12 : Noncopyable {
	public:
		/// <summary>
		/// SRVに登録。
		/// </summary>
		/// <param name="descriptorHandle"></param>
		virtual void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) = 0;
		virtual void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo) = 0;
	};
}