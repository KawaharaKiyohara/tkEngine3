#pragma once

#include "tkIGraphicsEngineImp.h"
#include "tkCamera.h"

namespace tkEngine {
	class CGameObjectManager;
	/// <summary>
	/// �O���t�B�b�N�G���W��
	/// </summary>
	class CGraphicsEngine : Noncopyable{
	public:
		/// <summary>
		/// �O���t�B�b�N�G���W�����������B
		/// </summary>
		/// <param name="hwnd">�E�B���h�E�n���h��</param>
		/// <param name="initParam">�������p�����[�^</param>
		/// <returns>false���Ԃ��Ă����珉�����Ɏ��s�B</returns>
		bool Init(HWND hwnd, const SInitParam& initParam);
		/// <summary>
		/// �`��B
		/// </summary>
		/// <param name="onRender">G-Buffer�ւ̃����_�����O�p�X�ŌĂ΂��֐��B</param>
		/// <param name="onPreForwardRender">�v���t�H���[�h�����_�����O�̃p�X�ŌĂ΂��֐��B</param>
		/// <param name="onForwardRender">�t�H���[�h�����_�����O�̃p�X�ŌĂ΂��֐��B</param>
		/// <param name="onPostRender">�|�X�g�����_�����O�̃p�X�ŌĂ΂��֐��B</param>
		void Render(
			std::function<void()> onRender,
			std::function<void()> onPreForwardRender, 
			std::function<void()> onForwardRender,
			std::function<void()> onPostRender
		);
		/// <summary>
		/// �j���B
		/// </summary>
		void Destroy();
		/// <summary>
		/// �t���[���o�b�t�@�̕����擾�B
		/// </summary>
		/// <returns>�t���[���o�b�t�@�̕��B</returns>
		int GetFrameBufferWidth() const
		{
			return m_frameBufferWidth;
		}
		/// <summary>
		/// �t���[���o�b�t�@�̍������擾�B
		/// </summary>
		/// <returns>�t���[���o�b�t�@�̍����B</returns>
		int GetFrameBufferHeight() const
		{
			return m_frameBufferHeight;
		}
		/// <summary>
		/// �O���t�B�b�N�X���\�[�X�̖����I�ȉ���B
		/// </summary>
		void Release()
		{
		}
		/// <summary>
		/// 3D�J�������擾�B
		/// </summary>
		/// <returns></returns>
		CCamera& GetCamera3D() 
		{
			return m_camera3D;
		}
		/// <summary>
		/// CGraphicsEngine�̎������擾�B
		/// </summary>
		template<class T>
		T* GetImplement()
		{
			return dynamic_cast<T*>(m_imp.get());
		}
	private:
		int	m_frameBufferWidth = 0;		//�t���[���o�b�t�@�̕��B
		int m_frameBufferHeight = 0;	//�t���[���o�b�t�@�̍����B
		CCamera m_camera3D;				//3D�J�����B	
		std::unique_ptr< IGraphicsEngineImp> m_imp;	//DirectX�̃o�[�W�����Ɉˑ�����G���W���̎����B
	};
}