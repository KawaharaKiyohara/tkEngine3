#pragma once

#pragma warning (disable  : 4201)
#pragma warning (disable  : 4127)
#pragma warning (disable :  4100)


#define TK_GRAPHICS_API_DIRECTX_9	9	// DirectX9
#define TK_GRAPHICS_API_DIRECTX_11	11	// DirectX11
#define TK_GRAPHICS_API_DIRECTX_12	12	// DirectX12
#define TK_GRAPHICS_API_OPENGL		100	// OpenGL

/// <summary>
/// �g�p���Ă���O���t�B�b�N�XAPI�̎�ށB
/// </summary>
#define TK_GRAPHICS_API		TK_GRAPHICS_API_DIRECTX_12

#define BUILD_LEVEL_DEBUG		0		//�f�o�b�O�r���h
#define BUILD_LEVEL_PREVIEW		1		//�v���r���[�r���h
#define BUILD_LEVEL_MASTER		2		//�}�X�^�[�r���h�B


/// <summary>
/// �r���h���x���B
/// </summary>
/// <remarks>
/// BUILD_LEVEL��BUILD_LEVEL_DEBUG�ABUILD_LEVEL_PREVIEW�ABUILD_LEVEL_MASTER
/// �̂R�̒l���Ƃ�܂��B���̂R�̒l�͉��L�̈Ӗ��������܂��B
/// 	BUILD_LEVEL_DEBUG		���S�ȃf�o�b�O�r���h�B
/// 	BUILD_LEVEL_PREVIEW		�R�[�h�͍œK������Ă��邪�A�f�o�b�O�@�\�͎c���Ă���B
/// 	BUILD_LEVEL_MASTER		�}�X�^�[�r���h�B�f�o�b�O�@�\�͊��S�ɍ폜����Ă���B
/// �f�o�b�O�R�[�h�𖄂ߍ��ގ��́ABUILD_LEVEL_MASTER�ł͖����ɂȂ�悤�Ɏ������Ă��������B
/// </remarks>
#ifdef _DEBUG
#define BUILD_LEVEL		BUILD_LEVEL_DEBUG
#elif defined(TK_MASTER)
#define BUILD_LEVEL		BUILD_LEVEL_MASTER
#else
#define BUILD_LEVEL		BUILD_LEVEL_PREVIEW
#endif


/// <summary>
/// �V�X�e���P�ʁB
/// </summary>
#define TK_SYSTEM_UNIT_CM		0		//��{�P��cm
#define TK_SYSTEM_UNIT_M		1		//��{�P��m

#define TK_SYSTEM_UNIT			TK_SYSTEM_UNIT_CM

namespace tkEngine {

	static constexpr inline float UnitCM(float param)
	{
#if TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_CM
		//��{�P�ʂ�cm�Ȃ炻�̂܂ܕԂ��B
		return param;
#elif TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_M
		//���[�g���P�ʂɖ߂��B
		return param * 0.01f;
#else
		TK_ASSERT(0, "�P�ʐݒ肪�s���ł��B");
		return 0.0f;
#endif
	}

	static constexpr inline float UnitM(float param)
	{
#if TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_CM
		//��{�P�ʂ�cm�Ȃ̂�*100
		return param * 100.0f;
#elif TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_M
		//���[�g���P�ʂȂ̂ł��̂܂܁B
		return param;
#else
		TK_ASSERT(0, "�P�ʐݒ肪�s���ł��B");
		return 0.0f;
#endif
	}

	static constexpr int NUM_SHADOW_MAP = 3;	//�V���h�E�}�b�v�̖����B
}
#include <WinSock2.h>
#include <windows.h>

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	#include <d3d12.h>
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include <D3Dcompiler.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <algorithm>
#include <map>
#include <array>
#include <functional>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <Xinput.h>
#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"


#include "Photon-cpp/Inc/PhotonPeer.h"

#include "tkEngine/tkEngineConst.h"
#include "tkEngine/policy/tkNoncopyable.h"
#include "tkEngine/policy/tkScopedResource.h"
#include "tkEngine/debug/tkLog.h"
#include "tkEngine/debug/tkAssert.h"
#include "tkEngine/math/tkMath.h"
#include "tkEngine/math/tkMatrix.h"

#include "gameObject/tkGameObjectManager.h"


#include "tkEngine/util/tkNameKey.h"
#include "tkEngine/timer/tkGameTime.h"
#include "tkEngine/Input/tkPad.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/sound/tkSoundSource.h"

