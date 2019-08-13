#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkIndexBuffer.h"


namespace tkEngine {
	IIndexBuffer::IIndexBuffer(int size, int stride)
	{
		m_count = size / stride;
	}

}