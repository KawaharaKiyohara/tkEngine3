#pragma once

#include "tkDescriptorHeapDx12.h"

namespace tkEngine {
	/// <summary>
	/// ディスクリプタヒープのプール。
	/// </summary>
	class CDescriptorHeapPoolDx12 : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		void Init();
		/// <summary>
		/// リセット。
		/// </summary>
		void Reset()
		{
			m_useDescriptor4Count = 0;
			m_useDescriptor8Count = 0;
			m_useDescriptor16Count = 0;
			m_useDescriptor32Count = 0;
		}
		/// <summary>
		/// CBR_SRV_UAV用のディスクリプタヒープを確保。
		/// </summary>
		/// <param name="numDescriptor">ディスクリプタの数</param>
		/// <returns></returns>
		CDescriptorHeapDx12& AllocCbrSrvUavDescriptorHeap(int numDescriptor)
		{
			CDescriptorHeapDx12* heap = nullptr;
			if (numDescriptor <= 4) {
				heap = &m_cbvSrvUavBufferDescriptor4.at(m_useDescriptor4Count);
				m_useDescriptor4Count++;
			}
			else if (numDescriptor <= 8) {
				heap = &m_cbvSrvUavBufferDescriptor8.at(m_useDescriptor8Count);
				m_useDescriptor8Count++;
			}
			else if (numDescriptor <= 16) {
				heap = &m_cbvSrvUavBufferDescriptor16.at(m_useDescriptor16Count);
				m_useDescriptor16Count++;
			}
			else if (numDescriptor <= 32) {
				heap = &m_cbvSrvUavBufferDescriptor32.at(m_useDescriptor32Count);
				m_useDescriptor32Count++;
			}
			else {
				TK_ASSERT(false, "ディスクリプタが足りない。");
			}
			return *heap;
		}
	private:
		//ディスクリプタの数。
		const int NUM_DESCRIPTOR_HEAP_4 = 32;
		const int NUM_DESCRIPTOR_HEAP_8 = 32;
		const int NUM_DESCRIPTOR_HEAP_16 = 32;
		const int NUM_DESCRIPTOR_HEAP_32 = 32;

		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor4;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor8;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor16;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor32;
		int m_useDescriptor4Count = 0;	//使用されているディスクリプタの数。
		int m_useDescriptor8Count = 0;	//使用されているディスクリプタの数。
		int m_useDescriptor16Count = 0;	//使用されているディスクリプタの数。
		int m_useDescriptor32Count = 0;	//使用されているディスクリプタの数。
	};
}