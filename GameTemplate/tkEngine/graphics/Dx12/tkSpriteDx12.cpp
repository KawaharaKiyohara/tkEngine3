#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkSpriteDx12.h"

namespace tkEngine {
	CSpriteDx12::~CSpriteDx12()
	{
	}
	void CSpriteDx12::OnInit(ITexture* texture, float w, float h)
	{
		//ディスクリプタヒープを初期化。
		InitDescriptorHeap();
		//定数バッファの作成。
		m_constantBufferGPU.Init(sizeof(m_constantBufferCPU), nullptr);
	}
	void CSpriteDx12::OnUpdate(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot)
	{
		
	}
	void CSpriteDx12::OnDraw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		m_constantBufferCPU.mvp = m_world * viewMatrix * projMatrix;
		m_constantBufferCPU.mulColor.x = 1.0f;
		m_constantBufferCPU.mulColor.y = 1.0f;
		m_constantBufferCPU.mulColor.z = 1.0f;
		m_constantBufferCPU.mulColor.w = 1.0f;

		m_constantBufferGPU.Update(&m_constantBufferCPU);

		auto& rc12 = renderContext.As<CRenderContextDx12>();
		//ルートシグネチャを設定。
		rc12.SetRootSignature(CPipelineStatesDx12::m_modelDrawRootSignature);
		//パイプラインステートを設定。
		rc12.SetPipelineState(CPipelineStatesDx12::m_spritePipeline);
		//頂点バッファを設定。
		rc12.SetVertexBuffer(m_vertexBuffer);
		//インデックスバッファを設定。
		rc12.SetIndexBuffer(m_indexBuffer);
		//プリミティブトポロジーを設定する。
		rc12.SetPrimitiveTopology(enPrimitiveTopology_TriangleStrip);

		//定数バッファとシェーダーリソースを設定。
		auto& texture = m_texture->As<CTextureDx12 >();
		IShaderResourceDx12* srvTbl[] = {
			&texture
		};
		CConstantBufferDx12* cbrTbl[] = {
			&m_constantBufferGPU
		};
		rc12.SetCBR_SRV_UAV(
			m_descriptorHeap.Get(),
			ARRAYSIZE(cbrTbl),
			cbrTbl,
			ARRAYSIZE(srvTbl),
			srvTbl);

		//ドロー。
		rc12.DrawIndexed(m_indexBuffer->GetCount());
	}

	void CSpriteDx12::InitDescriptorHeap()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 8;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");

	}

}