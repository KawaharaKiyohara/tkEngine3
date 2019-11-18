#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkDescriptorHeapDx12.h"


namespace tkEngine {

	void CDescriptorHeapDx12::Init(D3D12_DESCRIPTOR_HEAP_TYPE type, int numDescriptor)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = numDescriptor;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");


	}
}