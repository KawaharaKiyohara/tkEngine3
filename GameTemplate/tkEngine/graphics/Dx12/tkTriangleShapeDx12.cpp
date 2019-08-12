#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/dx12/tkTriangleShapeDx12.h"
#include "tkEngine/graphics/Dx12/d3dx12.h"
#include "tkEngine/graphics/Dx12/tkGraphicsEngineDx12.h"

namespace tkEngine {
	CTriangleShapeDx12::CTriangleShapeDx12()
	{
		auto gfxEngineDx12 = g_graphicsEngine->GetImplement<CGraphicsEngineDx12>();
		auto d3dDevice = gfxEngineDx12->GetD3DDevice();
		//空のルートシグネチャを作成。
		{
			
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
			d3dDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
		}
		//パイプラインステートの作成とシェーダーのコンパイル。
		{
			ComPtr<ID3DBlob> vertexShader;
			ComPtr<ID3DBlob> pixelShader;

#if BUILD_LEVEL != BUILD_LEVEL_MASTER
			// Enable better shader debugging with the graphics debugging tools.
			UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
			UINT compileFlags = 0;
#endif
			D3DCompileFromFile(L"shader/Primitive.fx", nullptr, nullptr, "VSMainNoTexture", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
			D3DCompileFromFile(L"shader/Primitive.fx", nullptr, nullptr, "PSMainNoTexture", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
		}
	}
}

#endif 