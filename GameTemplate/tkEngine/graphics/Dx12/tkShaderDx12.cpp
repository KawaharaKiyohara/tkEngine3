#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"

namespace tkEngine {
	void CShaderDx12::Load(const wchar_t* filePath, const char* entryFuncName, const char* shaderModel)
	{
		ComPtr<ID3DBlob> errorBlob;
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		auto hr = D3DCompileFromFile(filePath, nullptr, nullptr, entryFuncName, shaderModel, compileFlags, 0, &m_blob, &errorBlob);
		if (FAILED(hr)) {
			if (errorBlob) {
				static char errorMessage[10 * 1024];
				sprintf(errorMessage, "filePath : %s, %s", "hoge", (char*)errorBlob->GetBufferPointer());
				MessageBox(NULL, errorMessage, "シェーダーコンパイルエラー", MB_OK);
				return;
			}
		}
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


