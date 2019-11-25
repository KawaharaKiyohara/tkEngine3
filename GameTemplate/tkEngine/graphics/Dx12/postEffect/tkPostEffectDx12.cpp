#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkPostEffectDx12.h"

namespace tkEngine {
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	void CPostEffectDx12::Init(const SGraphicsConfig& config)
	{
		//ブルームを初期化。
		m_bloom.Init(config);
		//トーンマップを初期化。
		m_tonemap.Init(config);

		//四角形プリミティブを初期化。
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(1.0f, -1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(1.0f, 1.0f, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		m_quadPrimitive.Init(
			vertices,
			sizeof(vertices),
			sizeof(SSimpleVertex),
			indices,
			sizeof(indices),
			2,
			enPrimitiveTopology_TriangleStrip);
	}

	void CPostEffectDx12::Render(IRenderContext& rc)
	{
		auto& rc12 = rc.As<CRenderContextDx12>();
		//頂点バッファを設定。
		rc12.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//インデックスバッファを設定。
		rc12.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//プリミティブトポロジーを設定する。
		rc12.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());

		//トーンマップ。
		m_tonemap.Render(rc);
		//ブルーム。
		m_bloom.Render(rc);
	}
}