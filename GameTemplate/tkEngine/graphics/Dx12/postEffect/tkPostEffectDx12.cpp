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
		//�u���[�����������B
		m_bloom.Init(config);
		//�g�[���}�b�v���������B
		m_tonemap.Init(config);

		//�l�p�`�v���~�e�B�u���������B
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
		//���_�o�b�t�@��ݒ�B
		rc12.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		rc12.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		rc12.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());

		//�g�[���}�b�v�B
		m_tonemap.Render(rc);
		//�u���[���B
		m_bloom.Render(rc);
	}
}