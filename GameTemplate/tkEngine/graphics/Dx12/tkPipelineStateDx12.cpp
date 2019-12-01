#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkPipelineStateDx12.h"

namespace tkEngine {
	void CPipelineStateDx12::Init(D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		d3dDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState));

	}
}

#endif
