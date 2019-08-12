#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkGraphicsEngine.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>

namespace tkEngine {
	/// <summary>
	/// DirectX12�Ɉˑ�����O���t�B�b�N�X�G���W��
	/// </summary>
	class CGraphicsEngineDx12 : public IGraphicsEngine {
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�̃p�����[�^</param>
		/// <returns>false���Ԃ��Ă����珉�����Ɏ��s�B</returns>
		bool Init(HWND hwnd, const SInitParam& initParam) override final;
		/// <summary>
		/// �I�������B�A�v���P�[�V�������I������Ƃ��ɌĂяo���Ă��������B
		/// </summary>
		void Destroy() override final;
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="onRender">G-Buffer�ւ̃����_�����O�p�X�ŌĂ΂��֐��B</param>
		/// <param name="onPreForwardRender">�v���t�H���[�h�����_�����O�̃p�X�ŌĂ΂��֐��B</param>
		/// <param name="onForwardRender">�t�H���[�h�����_�����O�̃p�X�ŌĂ΂��֐��B</param>
		/// <param name="onPostRender">�|�X�g�����_�����O�̃p�X�ŌĂ΂��֐��B</param>

		void Render(
			std::function<void(CRenderContext& rc)> onRender,
			std::function<void(CRenderContext& rc)> onPreForwardRender,
			std::function<void(CRenderContext& rc)> onForwardRender,
			std::function<void(CRenderContext& rc)> onPostRender
		) override final;
		/// <summary>
		/// D3D�f�o�C�X���擾�B
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12Device> GetD3DDevice()
		{
			return m_d3dDevice;
		}
	private:
		/// <summary>
		/// 1�t���[���`��J�n���̏����B
		/// </summary>
		void BeginRender();
		/// <summary>
		/// 1�t���[���̕`��I�����̏����B
		/// </summary>
		void EndRender();
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
		/// <summary>
		/// �t���[���o�b�t�@�̂��߂̃����_�����O�^�[�Q�b�g�r���[���쐬����B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateRTVForFameBuffer();
		/// <summary>
		/// �R�}���h���X�g�̍쐬�B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateCommandList();
		/// <summary>
		/// GPU�Ɠ������Ƃ�I�u�W�F�N�g���쐬����B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateSynchronizationWithGPUObject();
		/// <summary>
		/// 1�t���[���O�̕`�悪�I���̂�҂B
		/// </summary>
		void WaitDraw();
		
	private:
		enum {
			FRAME_COUNT = 2
		};
		D3D12_VIEWPORT						m_viewport;			//�r���[�|�[�g�B
		D3D12_RECT							m_scissorRect;		//�V�U�����O��`�B
		ComPtr<ID3D12Device>				m_d3dDevice;		//D3D�f�o�C�X�B
		ComPtr<ID3D12CommandQueue>			m_commandQueue;		//�R�}���h�L���[�B
		ComPtr< IDXGISwapChain3>			m_swapChain;		//�X���b�v�`�F�C���B
		ComPtr< ID3D12DescriptorHeap>		m_rtvHeap;			//�t���[���o�b�t�@��RTV�p�̃f�B�X�N���v�^�q�[�v�B
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;	//�R�}���h�A���P�[�^�B
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;		//�R�}���h���X�g�B
		ComPtr<ID3D12PipelineState>			m_pipelineState;	//�p�C�v���C���X�e�[�g�B
		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;
		UINT m_rtvDescriptorSize = 0;						//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
		int m_currentBackBufferIndex = 0;					//���݂̃o�b�N�o�b�t�@�̔ԍ��B
		std::array< ComPtr<ID3D12Resource>, FRAME_COUNT> m_renderTargets;	//�t���[���o�b�t�@���̃����_�����O�^�[�Q�b�g�B
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12