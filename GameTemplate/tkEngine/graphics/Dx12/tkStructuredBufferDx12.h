#pragma once


namespace tkEngine {
	class CGraphicsEngineDx12;
	/// <summary>
	/// 構造化バッファ
	/// </summary>
	class CStructuredBufferDx12 : public IShaderResourceDx12 {
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~CStructuredBufferDx12();
		/// <summary>
		/// 構造化バッファを初期化。
		/// </summary>
		/// <param name="sizeOfElement">エレメントのサイズ。</param>
		/// <param name="numElement">エレメントの数。</param>
		/// <param name="initData">初期データ。</param>
		void Init(int sizeOfElement, int numElement, void* initData);
		/// <summary>
		/// SRVに登録。
		/// </summary>
		/// <param name="descriptorHandle"></param>
		void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle) override final;
		void RegistShaderResourceView(D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo) override final;
		/// <summary>
		/// 構造化バッファの内容を更新。
		/// </summary>
		/// <param name="data"></param>
		void Update(void* data);
		/// <summary>
		/// 初期化されているか判定。
		/// </summary>
		/// <returns></returns>
		bool IsInited() const
		{
			return m_isInited;
		}
	private:
		ComPtr<ID3D12Resource>	m_buffersOnGPU[2];
		void* m_buffersOnCPU[2] = { nullptr };		//CPU側からアクセスできるするストラクチャバッファのアドレス。
		int m_numElement = 0;				//要素数。
		int m_sizeOfElement = 0;			//エレメントのサイズ。
		bool m_isInited = false;			//初期化済み？
	};
}