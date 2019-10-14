#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkTkaFile.h"
#include <errno.h>
namespace tkEngine {
	void CTkaFile::LoadImplement(const char* filePath)
	{
		auto fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			TK_WARNING_MESSAGE_BOX_W(L"アニメーションクリップのオープンに失敗しました。ファイルパス : %s\n"
				L"原因として、下記の２点が考えられます。\n"
				L"① ファイルパスが間違えている。\n"
				L"② Assetsフォルダの中にファイルが存在しない。\n"
				L"この２点を確認して、問題が存在しない場合は、一度VisualStudioのビルド/リビルドを行ってみてください。\n", filePath);

			return;
		}
		//アニメーションクリップのヘッダーをロード。
		AnimClipHeader header;
		fread(&header, sizeof(header), 1, fp);

		if (header.numAnimationEvent > 0) {
			//アニメーションイベントがあるなら、イベント情報をロードする。
			for (auto i = 0; i < (int)header.numAnimationEvent; i++) {
				//イベントが起動する時間を読み込む。
				float invokeTime = 0.0f;
				fread(&invokeTime, sizeof(invokeTime), 1, fp);
				//イｂ根とメイの長さを読み込む。
				std::uint32_t eventNameLength;
				fread(&eventNameLength, sizeof(eventNameLength), 1, fp);
				//イベント名をロードする。
				static char eventName[256];
				static wchar_t wEventName[256];
				fread(eventName, eventNameLength + 1, 1, fp);
				AnimationEvent animEvent;
				animEvent.invokeTime = invokeTime;
				animEvent.eventName = eventName;
				m_animationEvents.push_back(std::move(animEvent));
			}
		}
		//キーフレームの情報をごそっとロード。
		m_keyFrames.resize(header.numKey);

		fread(&m_keyFrames.front(), sizeof(KeyFrame)* header.numKey, 1, fp);
	
		fclose(fp);
		//読み込み完了の印。
		SetLoadedMark();
	}
}