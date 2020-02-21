#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkSpriteDx12.h"

namespace tkEngine {
	CSpriteDx12::~CSpriteDx12()
	{
	}
	void CSpriteDx12::OnInit(ITexture* texture, float w, float h)
	{
		//定数バッファの作成。
		m_constantBufferGPU.Init(sizeof(m_constantBufferCPU), nullptr);
		//ディスクリプタヒープを初期化。
		InitDescriptorHeap();
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
	
		//頂点バッファを設定。
		rc12.SetVertexBuffer(m_vertexBuffer);
		//インデックスバッファを設定。
		rc12.SetIndexBuffer(m_indexBuffer);
		//プリミティブトポロジーを設定する。
		rc12.SetPrimitiveTopology(enPrimitiveTopology_TriangleStrip);

		rc12.SetDescriptorHeap(m_descriptorHeap);

		//定数バッファとシェーダーリソースを設定。
		auto& texture = m_texture->As<CTextureDx12 >();
	
		rc12.SetShaderResource(0, texture);
	
		rc12.SetConstantBuffer(0, m_constantBufferGPU);
	
		//ドロー。
		rc12.DrawIndexedFast(m_indexBuffer->GetCount());
	}

	void CSpriteDx12::InitDescriptorHeap()
	{
		m_descriptorHeap.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_descriptorHeap.RegistConstantBuffer(0, m_constantBufferGPU);
		auto& texture = m_texture->As<CTextureDx12 >();
		m_descriptorHeap.RegistShaderResource(0, texture);
		m_descriptorHeap.Commit();
	}

}