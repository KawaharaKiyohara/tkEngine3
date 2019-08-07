#pragma once

#include "tkEngine/graphics/tkIGraphicsEngineImp.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>

namespace tkEngine {
	/// <summary>
	/// DirectX12�Ɉˑ�����O���t�B�b�N�X�G���W���̎���
	/// </summary>
	class CGraphicsEngineDx12 : public IGraphicsEngineImp {
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�̃p�����[�^</param>
		/// <returns>false���Ԃ��Ă����珉�����Ɏ��s�B</returns>
		bool Init(HWND hwnd, const SInitParam& initParam) override final;
	private:
		/// <summary>
		/// DXGI�t�@�N�g���̍쐬�B
		/// </summary>
		/// <returns>�쐬���ꂽDXGI�t�@�N�g���B</returns>
		ComPtr<IDXGIFactory4> CreateDXGIFactory();
		/// <summary>
		/// D3D�f�o�C�X�̍쐬�B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateD3DDevice();
		/// <summary>
		/// �R�}���h�L���[�̍쐬�B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateCommandQueue();
		/// <summary>
		/// �X���b�v�`�F�C���̍쐬�B
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�����[�^�B</param>
		/// <param name="dxgiFactory">DXGI�t�@�N�g���[�B</param>
		/// <returns>false���Ԃ���������쐬�Ɏ��s�B</returns>
		bool CreateSwapChain(HWND hwnd, const SInitParam& initParam, ComPtr<IDXGIFactory4> dxgiFactory);
		/// <summary>
		/// �t���[���o�b�t�@�̂��߂̃f�B�X�N���v�^�q�[�v���쐬����B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateDescriptorHeapForFrameBuffer();
	private:
		ComPtr<ID3D12Device>		m_d3dDevice;	//D3D�f�o�C�X�B
		ComPtr<ID3D12CommandQueue>	m_commandQueue;	//�R�}���h�L���[�B
		ComPtr< IDXGISwapChain3>	m_swapChain;	//�X���b�v�`�F�C���B
		int m_currentBackBufferIndex = 0;			//���݂̃o�b�N�o�b�t�@�̔ԍ��B
	};
}