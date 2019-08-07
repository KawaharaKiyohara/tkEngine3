#pragma once

#include "tkIGraphicsEngineImp.h"
#include "tkCamera.h"

namespace tkEngine {
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
	private:
		int	m_frameBufferWidth = 0;		//�t���[���o�b�t�@�̕��B
		int m_frameBufferHeight = 0;	//�t���[���o�b�t�@�̍����B
		CCamera m_camera3D;				//3D�J�����B	
		std::unique_ptr< IGraphicsEngineImp> m_graphicsEngineImp;	//DirectX�̃o�[�W�����Ɉˑ�����G���W���̎����B
	};
}