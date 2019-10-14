#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkTkaFile.h"
#include <errno.h>
namespace tkEngine {
	void CTkaFile::LoadImplement(const char* filePath)
	{
		auto fp = fopen(filePath, "rb");
		if (fp == nullptr) {
			TK_WARNING_MESSAGE_BOX_W(L"�A�j���[�V�����N���b�v�̃I�[�v���Ɏ��s���܂����B�t�@�C���p�X : %s\n"
				L"�����Ƃ��āA���L�̂Q�_���l�����܂��B\n"
				L"�@ �t�@�C���p�X���ԈႦ�Ă���B\n"
				L"�A Assets�t�H���_�̒��Ƀt�@�C�������݂��Ȃ��B\n"
				L"���̂Q�_���m�F���āA��肪���݂��Ȃ��ꍇ�́A��xVisualStudio�̃r���h/���r���h���s���Ă݂Ă��������B\n", filePath);

			return;
		}
		//�A�j���[�V�����N���b�v�̃w�b�_�[�����[�h�B
		AnimClipHeader header;
		fread(&header, sizeof(header), 1, fp);

		if (header.numAnimationEvent > 0) {
			//�A�j���[�V�����C�x���g������Ȃ�A�C�x���g�������[�h����B
			for (auto i = 0; i < (int)header.numAnimationEvent; i++) {
				//�C�x���g���N�����鎞�Ԃ�ǂݍ��ށB
				float invokeTime = 0.0f;
				fread(&invokeTime, sizeof(invokeTime), 1, fp);
				//�C�����ƃ��C�̒�����ǂݍ��ށB
				std::uint32_t eventNameLength;
				fread(&eventNameLength, sizeof(eventNameLength), 1, fp);
				//�C�x���g�������[�h����B
				static char eventName[256];
				static wchar_t wEventName[256];
				fread(eventName, eventNameLength + 1, 1, fp);
				AnimationEvent animEvent;
				animEvent.invokeTime = invokeTime;
				animEvent.eventName = eventName;
				m_animationEvents.push_back(std::move(animEvent));
			}
		}
		//�L�[�t���[���̏����������ƃ��[�h�B
		m_keyFrames.resize(header.numKey);

		fread(&m_keyFrames.front(), sizeof(KeyFrame)* header.numKey, 1, fp);
	
		fclose(fp);
		//�ǂݍ��݊����̈�B
		SetLoadedMark();
	}
}