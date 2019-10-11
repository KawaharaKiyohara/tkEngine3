#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/Resource/tkIResource.h"

namespace tkEngine {
	IResource::~IResource()
	{
		if (m_loadThread) {
			//読み込みスレッドが終わるまで待機。
			m_loadThread->join();
		}
	}
	void IResource::LoadAsync(const char* filePath)
	{
		m_filePath = filePath;
		m_loadThread = std::make_unique<std::thread>(
			[&]() {	Load(m_filePath.c_str());  });
	}
}
