#pragma once

#include "tkEngine/graphics/tkGraphicsConfig.h"

namespace tkEngine{
	/// <summary>
	/// エンジン初期化用のパラメータ。
	/// </summary>
	struct SInitParam{
		SInitParam(){
			gameObjectPrioMax = 32;
		}
		HINSTANCE hInstance = nullptr;		//!<アプリケーションインスタンス。
		SGraphicsConfig graphicsConfing;	//グラフィックス設定。
		int	screenWidth = 0;				//!<スクリーンの幅。
		int	screenHeight = 0;				//!<スクリーンの高さ。
		int frameBufferHeight = 0;			//!<フレームバッファの幅。これが内部解像度。
		int frameBufferWidth = 0;			//!<フレームバッファの高さ。これが内部解像度。
		int nCmdShow = 0;						//!<
		std::uint8_t gameObjectPrioMax = 32;
	};
}