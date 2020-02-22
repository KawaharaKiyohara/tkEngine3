#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "dx12Common.h"
#include "tkMeshPartsDx12.h"
#include "tkLightManagerDx12.h"
#include "tkDirectionalShadowMapDx12.h"

namespace tkEngine {
	UPIGraphicsEngine CGraphicsInstanceFactoryDx12::CreateGraphicsEngine()
	{
		return make_unique< CGraphicsEngineDx12>();
	}
	UPIVertexBuffer CGraphicsInstanceFactoryDx12::CreateVertexBuffer(int size, int stride)
	{
		auto vb = make_unique<CVertexBufferDx12>();
		vb->Init(size, stride);
		return vb;
	}
	UPIIndexBuffer CGraphicsInstanceFactoryDx12::CreateIndexBuffer(int size, int stride)
	{
		auto ib = make_unique<CIndexBufferDx12>();
		ib->Init(size, stride);
		return ib;
	}
	UPIRenderContext CGraphicsInstanceFactoryDx12::CreateRenderContext()
	{
		return make_unique<CRenderContextDx12>();
	}
	UPITexture CGraphicsInstanceFactoryDx12::CreateTextureFromDDSFile(const wchar_t* filePath)
	{
		return make_unique<CTextureDx12>(filePath);
	}
	UPIMeshParts CGraphicsInstanceFactoryDx12::CreateMeshPartsFromTkmFile(const CTkmFile& tkmFile)
	{
		auto meshParts = make_unique<CMeshPartsDx12>();
		meshParts->InitFromTkmFile(tkmFile);
		return meshParts;
	}
	UPILightManager CGraphicsInstanceFactoryDx12::CreateLightManager()
	{
		return make_unique<CLightManagerDx12>();
	}
	UPIDirectionalShadowMap CGraphicsInstanceFactoryDx12::CreateDirectionalShadowMap()
	{
		return make_unique<CDirectionalShadowMapDx12>();
	}
}

#endif
