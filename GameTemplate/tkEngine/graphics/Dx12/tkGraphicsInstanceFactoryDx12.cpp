#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "dx12Common.h"
#include "tkTriangleShapeDx12.h"
#include "tkMeshPartsDx12.h"

namespace tkEngine {
	UPIGraphicsEngine CGraphicsInstanceFactoryDx12::CreateGraphicsEngine()
	{
		return std::make_unique< CGraphicsEngineDx12>();
	}
	UPIVertexBuffer CGraphicsInstanceFactoryDx12::CreateVertexBuffer(int size, int stride)
	{
		auto vb = std::make_unique<CVertexBufferDx12>();
		vb->Init(size, stride);
		return vb;
	}
	UPIIndexBuffer CGraphicsInstanceFactoryDx12::CreateIndexBuffer(int size, int stride)
	{
		auto ib = std::make_unique<CIndexBufferDx12>();
		ib->Init(size, stride);
		return ib;
	}
	UPIRenderContext CGraphicsInstanceFactoryDx12::CreateRenderContext()
	{
		return std::make_unique<CRenderContextDx12>();
	}
	UPIShape CGraphicsInstanceFactoryDx12::CreateTriangleShape(const wchar_t* textureFilePath)
	{
		return std::make_unique<CTriangleShapeDx12>(textureFilePath);
	}
	UPITexture CGraphicsInstanceFactoryDx12::CreateTextureFromDDSFile(const wchar_t* filePath)
	{
		return std::make_unique<CTextureDx12>(filePath);
	}
	UPIMeshParts CGraphicsInstanceFactoryDx12::CreateMeshPartsFromTkmFile(const CTkmFile& tkmFile)
	{
		auto meshParts = std::make_unique<CMeshPartsDx12>();
		meshParts->InitFromTkmFile(tkmFile);
		return meshParts;
	}
}

#endif
