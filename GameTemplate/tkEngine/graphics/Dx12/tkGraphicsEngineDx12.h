#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkGraphicsEngine.h"
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include "tkRenderTargetDx12.h"
#include "tkSpriteDx12.h"

#include "postEffect/tkPostEffectDx12.h"

namespace tkEngine {
	/// <summary>
	/// DirectX12�Ɉˑ�����O���t�B�b�N�X�G���W��
	/// </summary>
	class CGraphicsEngineDx12 : public IGraphicsEngine {
	public:
		enum {
			FRAME_COUNT = 2
		};
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�̃p�����[�^</param>
		/// <returns>false���Ԃ��Ă����珉�����Ɏ��s�B</returns>
		bool OnInit(HWND hwnd, const SInitParam& initParam) override final;
		/// <summary>
		/// �I�������B�A�v���P�[�V�������I������Ƃ��ɌĂяo���Ă��������B
		/// </summary>
		void Destroy() override final;
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="goMgr">�Q�[���I�u�W�F�N�g�}�l�[�W��</param>
		void OnRender(CGameObjectManager* goMgr) override final;
		/// <summary>
		/// D3D�f�o�C�X���擾�B
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12Device> GetD3DDevice() const
		{
			return m_d3dDevice;
		}
		/// <summary>
		/// �R�}���h�L���[���擾�B
		/// </summary>
		/// <returns></returns>
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const
		{
			return m_commandQueue;
		}
		/// <summary>
		/// �o�b�N�o�b�t�@�̔ԍ����擾�B
		/// </summary>
		/// <returns>�o�b�N�o�b�t�@�̔ԍ��B</returns>
		UINT GetBackBufferIndex() const
		{
			return m_frameIndex;
		}
		/// <summary>
		/// CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y���擾�B
		/// </summary>
		/// <returns></returns>
		UINT GetCbrSrvDescriptorSize() const
		{
			return m_cbrSrvDescriptorSize;
		}
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g���擾�B
		/// </summary>
		/// <returns></returns>
		CRenderTargetDx12& GetMainRenderTarget()
		{
			return m_mainRenderTarget;
		}
		/// <summary>
		/// ���C�������_�����O�^�[�Q�b�g�̃r���[�|�[�g���擾�B
		/// </summary>
		/// <returns></returns>
		D3D12_VIEWPORT& GetMainRenderTargetViewport()
		{
			return m_viewport;
		}

	private:
		/// <summary>
		/// 1�t���[���`��J�n���̏����B
		/// </summary>
		void OnBeginRender() override final;
		/// <summary>
		/// 1�t���[���̕`��I�����̏����B
		/// </summary>
		void OnEndRender() override final;
		/// <summary>
		/// �v�������_�����O�J�n���ɌĂ΂�鏈���B
		/// </summary>
		void OnBeginPreRender()override final;
		/// <summary>
		/// �v�������_�����O�I�����ɌĂ΂�鏈���B
		/// </summary>
		void OnEndPreRender() override final;
		/// <summary>
		/// DXGI�t�@�N�g���̍쐬�B
		/// </summary>
		/// <returns>�쐬���ꂽDXGI�t�@�N�g���B</returns>
		ComPtr<IDXGIFactory4> CreateDXGIFactory();
		/// <summary>
		/// D3D�f�o�C�X�̍쐬�B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateD3DDevice(ComPtr<IDXGIFactory4> dxgiFactory);
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
		/// �t���[���o�b�t�@�̂��߂̐[�x�X�e���V���r���[���쐬����B
		/// </summary>
		/// <returns>false���Ԃ��Ă�����쐬�Ɏ��s�B</returns>
		bool CreateDSVForFrameBuffer(const SInitParam& initParam);
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
		D3D12_VIEWPORT						m_viewport;				//�r���[�|�[�g�B
		D3D12_RECT							m_scissorRect;			//�V�U�����O��`�B
		ComPtr<ID3D12Device>				m_d3dDevice;			//D3D�f�o�C�X�B
		ComPtr<ID3D12CommandQueue>			m_commandQueue;			//�R�}���h�L���[�B
		ComPtr< IDXGISwapChain3>			m_swapChain;			//�X���b�v�`�F�C���B
		ComPtr< ID3D12DescriptorHeap>		m_rtvHeap;				//�t���[���o�b�t�@��RTV�p�̃f�B�X�N���v�^�q�[�v�B
		ComPtr< ID3D12DescriptorHeap>		m_dsvHeap;				//�t���[���o�b�t�@�p�̐[�x�X�e���V���o�b�t�@�r���[�̃f�B�X�N���v�^�q�[�v�B
		ComPtr<ID3D12CommandAllocator>		m_commandAllocator;		//�R�}���h�A���P�[�^�B
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;			//�R�}���h���X�g�B
		ComPtr<ID3D12PipelineState>			m_pipelineState;		//�p�C�v���C���X�e�[�g�B
		CRenderTargetDx12					m_mainRenderTarget;		//���C�������_�����O�^�[�Q�b�g�B
		CSpriteDx12 m_copyFullScreenSprite;	//�t���X�N���[���R�s�[�p�̃X�v���C�g�B
		CPostEffectDx12 m_postEffect;
		
		// Synchronization objects.
		UINT m_frameIndex;
		HANDLE m_fenceEvent;
		ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValue;
		UINT m_rtvDescriptorSize = 0;						//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
		UINT m_dsvDescriptorSize = 0;						//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
		UINT m_cbrSrvDescriptorSize = 0;					//CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y�B
		int m_currentBackBufferIndex = 0;					//���݂̃o�b�N�o�b�t�@�̔ԍ��B
		std::array< ComPtr<ID3D12Resource>, FRAME_COUNT> m_renderTargets;	//�t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�B
		ComPtr<ID3D12Resource> m_depthStencilBuffer;	//�[�x�X�e���V���o�b�t�@�B
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12