/*!
 *@file	tkEngine.h
 */
#pragma once

#include "tkEngine/timer/tkStopwatch.h"
#include "tkEngine/Sound/tkSoundEngine.h"
#include "tkEngine/tkEngineStruct.h"
#include "tkEngine/random/tkRandom.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "tkEngine/graphics/tkGraphicsEngine.h"
#include "tkEngine/graphics/tkGraphicsInstanceFactory.h"

 /*!
  *@namespace	tkEngine�S�̖̂��O��ԁB
  */
namespace tkEngine{
	/*!
	 *@brief	�͌��w�������Q�[���G���W�� version 3
	 */
	class CEngine : Noncopyable {
	private:
		/*!
		 *@brief	�R���X�g���N�^�B
		 */
		CEngine();
		/*!
		 *@brief	�f�X�g���N�^�B
		 */
		~CEngine();
	public:
		/*!
		 *@brief	�������B
		 *@param[in]	initParam		�������p�����[�^�B
		 */
		static bool Init(const SInitParam& initParam);
		/*!
		 *@brief	�I�������B
		 */
		void Final();
		/*!
		 *@brief	�Q�[�����[�v�����s�B
		 */
		void RunGameLoop();
		/*!
		 *@brief	�C���X�^���X���擾�B
		 */
		static CEngine& GetInstance()
		{
			static CEngine* instance = nullptr;
			if (instance == nullptr) {
				instance = new CEngine;
			}
			return *instance;
		}

		/*!
		* @brief	�Q�[���p�b�h�̎擾�B
		*@param[in]	padNo	�p�b�h�ԍ��BCPad::CONNECT_PAD_MAX-1�܂Ŏw��\�B
		*/
		CPad& GetPad(int padNo)
		{
			TK_ASSERT(padNo < CPad::CONNECT_PAD_MAX && padNo >= 0, "padNo is invalid");
			return m_pad[padNo];
		}
		/*!
		* @brief	�T�E���h�G���W���̎擾�B
		*/
		CSoundEngine& GetSoundEngine()
		{
			return m_soundEngine;
		}
		
		/*!
		 *@brief	�����擾�B
		 */
		CRandom& GetRandom()
		{
			return m_random;
		}
		/*!
		*@brief		���\�[�X�̃A�����[�h�B
		*@details
		* �풓���\�[�X�ȊO�̃��\�[�X�����ׂăA�����[�h���܂��B
		* �V�[���̐؂�ւ��ȂǂŁA���\�[�X�̃A�����[�h���K�v�ȏꍇ�Ɏg�p���Ă��������B
		*/
		void ResourceUnload();

		/// <summary>
		/// �O���t�B�b�N�X�C���X�^���X�̃t�@�N�g�����擾�B
		/// </summary>
		/// <returns></returns>
		IGraphicsInstanceFactory* GetGraphicsInstanceFactory()
		{
			return m_graphisInstanceFactory.get();
		}
	private:
		/// <summary>
		/// �����̏���������
		/// </summary>
		/// <param name="initParam">�������p�����[�^�B</param>
		/// <returns></returns>
		bool InitInternal(const SInitParam& initParam);
		/*!
		 *@brief	�E�B���h�E�������B
		 */
		bool InitWindow( const SInitParam& initParam );
		
		/// <summary>
		/// �Q�[���X���b�h�B
		/// </summary>
		void GameThread();
		/*!
		* @brief	�E�B���h�E�v���V�[�W���B
		*@param[in]	hWnd	�E�B���h�E�n���h��
		*@param[in] msg		���b�Z�[�W
		*@param[in] wParam	WPARAM
		*@param[in] lParam	LPARAM
		*/
		static LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		/*!
		* @brief	�X�V�B
		*/
		void Update();
		/// <summary>
		/// �Q�[�������̍X�V�����B
		/// </summary>
		void GameUpdate();
	private:
		HINSTANCE				m_hInst = nullptr;	//!<�A�v���P�[�V�����̃C���X�^���X�B
		HWND					m_hWnd = nullptr;	//!<�E�B���h�E�n���h���B
		CSoundEngine			m_soundEngine;								//!<�T�E���h�G���W���B
		UPIGraphicsInstanceFactory m_graphisInstanceFactory;				//�O���t�B�b�N�X�C���X�^���X�̃t�@�N�g���B
		UPIGraphicsEngine		m_graphicsEngine;							//�O���t�B�b�N�G���W���B
		CGameTime				m_gameTime;									//�Q�[���^�C���B
		CPhysicsWorld			m_physicsWorld;		//�������[���h�B
		int						m_screenWidth = 0;							//!<�X�N���[���̕��B
		int						m_screenHeight = 0;							//!<�X�N���[���̍����B
		CPad					m_pad[CPad::CONNECT_PAD_MAX];				//!<�Q�[���p�b�h�B
		CRandom					m_random;									//!<�����B
		std::unique_ptr<std::thread> m_gameThread;							//�Q�[���X���b�h�B
		bool			m_isRunGameThread = false;
		bool			m_isReqDeadGameThread = false;
		std::mutex		m_isRunGameThreadMtx;
		std::condition_variable m_isRunGameThreadCv;
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		float					m_fps = 30.0f;
		float					m_timeTotal = 0.0f;
#endif
	public:
		CStopwatch				m_sw;
	};
	//�G���W���̊O���ɃC���X�^���X�����J����O���[�o���ȃA�N�Z�X�|�C���g�B
	extern CEngine* g_engine;
	extern IGraphicsEngine* g_graphicsEngine;
	extern CCamera* g_camera3D;
	extern CCamera* g_camera2D;
	extern CGameTime* g_gameTime;
	extern ILightManager* g_lightManager;
	extern std::array<CPad*, CPad::CONNECT_PAD_MAX> g_pad;
	extern CPhysicsWorld* g_physicsWorld;

	/*!
	* @brief	CSoundEngine�̃C���X�^���X���擾�B
	*/
	static inline CSoundEngine& SoundEngine()
	{
		return g_engine->GetSoundEngine();
	}

	/*!
	 *@brief	�������擾�B
	 */
	static inline CRandom& Random()
	{
		return g_engine->GetRandom();
	}
	
}


