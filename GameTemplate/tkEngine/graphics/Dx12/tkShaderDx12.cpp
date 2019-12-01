#include "tkEngine/tkEnginePreCompile.h"
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"

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
	void CShaderDx12::LoadPS(const wchar_t* filePath, const char* entryFuncName)
	{
		Load(filePath, entryFuncName, g_psShaderModelName);
	}
	void CShaderDx12::LoadVS(const wchar_t* filePath, const char* entryFuncName)
	{
		Load(filePath, entryFuncName, g_vsShaderModelName);
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


