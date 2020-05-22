#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "dx12Common.h"


namespace tkEngine {
	void CIndexBufferDx12::InitSub(int size, int stride)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		auto hr = d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(size),
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_indexBuffer));
		//���_�o�b�t�@�̍쐬�Ɏ��s���܂����B
		TK_ASSERT(SUCCEEDED(hr), "�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B");
		//�C���f�b�N�X�o�b�t�@�̃r���[���쐬�B
		m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
		m_indexBufferView.SizeInBytes = size;
		if (stride == 2) {
			//�X�g���C�h���Q�o�C�g�B
			m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
		}
		else if (stride == 4) {
			//�X�g���C�h���S�o�C�g�B
			m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		}
	}
	void CIndexBufferDx12::Copy(void* srcIndecies)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		DirectX::ResourceUploadBatch re(d3dDevice.Get());
		re.Begin();
		D3D12_SUBRESOURCE_DATA subResourceData;
		subResourceData.pData = srcIndecies;
		subResourceData.RowPitch = m_indexBufferView.SizeInBytes;
		subResourceData.SlicePitch = 1;
		re.Upload(
			m_indexBuffer.Get(),
			0,
			&subResourceData,
			1);

		re.End(ge12.GetCommandQueue().Get());
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12