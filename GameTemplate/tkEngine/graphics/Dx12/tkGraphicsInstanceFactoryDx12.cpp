#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "dx12Common.h"
#include "tkTriangleShapeDx12.h"

namespace tkEngine {
	UPIGraphicsEngine CGraphicsInstanceFactoryDx12::CreateGraphicsEngineInstance()
	{
		return std::make_unique< CGraphicsEngineDx12>();
	}
	UPIVertexBuffer CGraphicsInstanceFactoryDx12::CreateVertexBufferInstance(int size, int stride)
	{
		return std::make_unique<CVertexBufferDx12>(size, stride);
	}
	UPIIndexBuffer CGraphicsInstanceFactoryDx12::CreateIndexBufferInstance(int size, int stride)
	{
		return std::make_unique<CIndexBufferDx12>(size, stride);
	}
	UPIRenderContext CGraphicsInstanceFactoryDx12::CreateRenderContextInstance()
	{
		return std::make_unique<CRenderContextDx12>();
	}
	UPIShape CGraphicsInstanceFactoryDx12::CreateTriangleShapeInstance(const wchar_t* textureFilePath)
	{
		return std::make_unique<CTriangleShapeDx12>(textureFilePath);
	}
	UPITexture CGraphicsInstanceFactoryDx12::CreateTextureFromDDSFile(const wchar_t* filePath)
	{
		return std::make_unique<CTextureDx12>(filePath);
	}
}

#endif
