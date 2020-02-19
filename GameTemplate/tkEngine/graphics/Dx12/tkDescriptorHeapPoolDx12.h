#pragma once

#include "tkDescriptorHeapDx12.h"

namespace tkEngine {
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̃v�[���B
	/// </summary>
	class CDescriptorHeapPoolDx12 : Noncopyable {
	public:
		/// <summary>
		/// �������B
		/// </summary>
		void Init();
		/// <summary>
		/// ���Z�b�g�B
		/// </summary>
		void Reset()
		{
			m_useDescriptor4Count = 0;
			m_useDescriptor8Count = 0;
			m_useDescriptor16Count = 0;
			m_useDescriptor32Count = 0;
		}
		/// <summary>
		/// CBR_SRV_UAV�p�̃f�B�X�N���v�^�q�[�v���m�ہB
		/// </summary>
		/// <param name="numDescriptor">�f�B�X�N���v�^�̐�</param>
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
				TK_ASSERT(false, "�f�B�X�N���v�^������Ȃ��B");
			}
			return *heap;
		}
	private:
		//�f�B�X�N���v�^�̐��B
		const int NUM_DESCRIPTOR_HEAP_4 = 32;
		const int NUM_DESCRIPTOR_HEAP_8 = 32;
		const int NUM_DESCRIPTOR_HEAP_16 = 32;
		const int NUM_DESCRIPTOR_HEAP_32 = 32;

		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor4;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor8;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor16;
		std::vector<CDescriptorHeapDx12> m_cbvSrvUavBufferDescriptor32;
		int m_useDescriptor4Count = 0;	//�g�p����Ă���f�B�X�N���v�^�̐��B
		int m_useDescriptor8Count = 0;	//�g�p����Ă���f�B�X�N���v�^�̐��B
		int m_useDescriptor16Count = 0;	//�g�p����Ă���f�B�X�N���v�^�̐��B
		int m_useDescriptor32Count = 0;	//�g�p����Ă���f�B�X�N���v�^�̐��B
	};
}