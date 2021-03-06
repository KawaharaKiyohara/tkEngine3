#pragma once

#pragma warning (disable  : 4201)
#pragma warning (disable  : 4127)
#pragma warning (disable :  4100)


#define TK_GRAPHICS_API_DIRECTX_9	9	// DirectX9
#define TK_GRAPHICS_API_DIRECTX_11	11	// DirectX11
#define TK_GRAPHICS_API_DIRECTX_12	12	// DirectX12
#define TK_GRAPHICS_API_OPENGL		100	// OpenGL

/// <summary>
/// 使用しているグラフィックスAPIの種類。
/// </summary>
#define TK_GRAPHICS_API		TK_GRAPHICS_API_DIRECTX_12

#define BUILD_LEVEL_DEBUG		0		//デバッグビルド
#define BUILD_LEVEL_PREVIEW		1		//プレビュービルド
#define BUILD_LEVEL_MASTER		2		//マスタービルド。


/// <summary>
/// ビルドレベル。
/// </summary>
/// <remarks>
/// BUILD_LEVELはBUILD_LEVEL_DEBUG、BUILD_LEVEL_PREVIEW、BUILD_LEVEL_MASTER
/// の３つの値をとります。この３つの値は下記の意味を持ちます。
/// 	BUILD_LEVEL_DEBUG		完全なデバッグビルド。
/// 	BUILD_LEVEL_PREVIEW		コードは最適化されているが、デバッグ機能は残っている。
/// 	BUILD_LEVEL_MASTER		マスタービルド。デバッグ機能は完全に削除されている。
/// デバッグコードを埋め込む時は、BUILD_LEVEL_MASTERでは無効になるように実装してください。
/// </remarks>
#ifdef _DEBUG
#define BUILD_LEVEL		BUILD_LEVEL_DEBUG
#elif defined(TK_MASTER)
#define BUILD_LEVEL		BUILD_LEVEL_MASTER
#else
#define BUILD_LEVEL		BUILD_LEVEL_PREVIEW
#endif


/// <summary>
/// システム単位。
/// </summary>
#define TK_SYSTEM_UNIT_CM		0		//基本単位cm
#define TK_SYSTEM_UNIT_M		1		//基本単位m

#define TK_SYSTEM_UNIT			TK_SYSTEM_UNIT_CM

namespace tkEngine {

	static constexpr inline float UnitCM(float param)
	{
#if TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_CM
		//基本単位がcmならそのまま返す。
		return param;
#elif TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_M
		//メートル単位に戻す。
		return param * 0.01f;
#else
		TK_ASSERT(0, "単位設定が不正です。");
		return 0.0f;
#endif
	}

	static constexpr inline float UnitM(float param)
	{
#if TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_CM
		//基本単位がcmなので*100
		return param * 100.0f;
#elif TK_SYSTEM_UNIT==TK_SYSTEM_UNIT_M
		//メートル単位なのでそのまま。
		return param;
#else
		TK_ASSERT(0, "単位設定が不正です。");
		return 0.0f;
#endif
	}

	static constexpr int NUM_SHADOW_MAP = 3;	//シャドウマップの枚数。
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
using namespace std;

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"


#include "Photon-cpp/Inc/PhotonPeer.h"

#include "DirectXTK/Inc/DDSTextureLoader.h"
#include "DirectXTK/Inc/ResourceUploadBatch.h"

#include "tkEngine/tkEngineConst.h"
#include "tkEngine/policy/tkNoncopyable.h"
#include "tkEngine/Resource/tkIResource.h"
#include "tkEngine/policy/tkScopedResource.h"
#include "tkEngine/debug/tkLog.h"
#include "tkEngine/debug/tkAssert.h"
#include "tkEngine/math/tkMath.h"
#include "tkEngine/math/tkMatrix.h"
#include "tkEngine/graphics/tkGraphics.h"
#include "tkEngine/graphics/tkRenderContext.h"

#include "gameObject/tkGameObjectManager.h"

#include "tkEngine/physics/tkPhysics.h"
#include "tkEngine/util/tkNameKey.h"
#include "tkEngine/timer/tkGameTime.h"
#include "tkEngine/Input/tkPad.h"
#include "tkEngine/tkEngine.h"
#include "tkEngine/sound/tkSoundSource.h"
#include "tkEngine/graphics/tkShape.h"
#include "tkEngine/graphics/tkTexture.h"
#include "tkEngine/graphics/tkSkeleton.h"
#include "tkEngine/graphics/tkAnimationClip.h"
#include "tkEngine/graphics/tkAnimationPlayController.h"
#include "tkEngine/graphics/tkAnimation.h"
#include "tkEngine/graphics/tkLightManager.h"
#include "tkEngine/graphics/tkDirectionalShadowMap.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/dx12/dx12Common.h"
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
