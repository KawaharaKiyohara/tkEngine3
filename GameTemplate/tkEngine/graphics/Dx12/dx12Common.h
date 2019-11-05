#pragma once

#include "d3dx12.h"

namespace tkEngine {

	class CIndexBufferDx12;
	using UPCIndexBufferDx12 = std::unique_ptr<CIndexBufferDx12>;
	static const char* g_vsShaderModelName = "vs_5_0"; //頂点シェーダーのシェーダーモデル名。
	static const char* g_psShaderModelName = "ps_5_0";	//ピクセルシェーダーのシェーダモデル名。
}
#include "tkGraphicsInstanceFactoryDx12.h"
#include "tkShaderResourceDx12.h"
#include "tkIndexBufferDx12.h"
#include "tkVertexBufferDx12.h"

#include "tkGraphicsEngineDx12.h"
#include "tkRootSignatureDx12.h"
#include "tkRenderContextDx12.h"
#include "tkShaderDx12.h"
#include "tkConstantBufferDx12.h"
#include "tkRootSignatureDx12.h"
#include "tkTextureDx12.h"

#include "tkStructuredBufferDx12.h"
#include "tkRenderContextDx12_inline.h"
#include "tkPipelineStatesDx12.h"

