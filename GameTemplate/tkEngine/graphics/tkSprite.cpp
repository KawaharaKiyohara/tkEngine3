#include "tkEngine/tkEnginePrecompile.h"
#include "tkSprite.h"
#include "tkVertexBuffer.h"
#include "tkIndexBuffer.h"

namespace tkEngine {
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	const CVector2	ISprite::DEFAULT_PIVOT = { 0.5f, 0.5f };
	ISprite::~ISprite()
	{
	}
	void ISprite::Init(ITexture* texture, float w, float h)
	{
		TK_ASSERT(texture != nullptr, "�e�N�X�`�����w�肵�Ă��������B");
		m_texture = texture;

		m_size.x = w;
		m_size.y = h;

		float halfW = w * 0.5f;
		float halfH = h * 0.5f;
		//���_�o�b�t�@�̃\�[�X�f�[�^�B
		SSimpleVertex vertices[] =
		{
			{
				CVector4(-halfW, -halfH, 0.0f, 1.0f),
				CVector2(0.0f, 1.0f),
			},
			{
				CVector4(halfW, -halfH, 0.0f, 1.0f),
				CVector2(1.0f, 1.0f),
			},
			{
				CVector4(-halfW, halfH, 0.0f, 1.0f),
				CVector2(0.0f, 0.0f)
			},
			{
				CVector4(halfW, halfH, 0.0f, 1.0f),
				CVector2(1.0f, 0.0f)
			}

		};
		short indices[] = { 0,1,2,3 };
		auto gfxFactory = g_engine->GetGraphicsInstanceFactory();
		m_vertexBuffer = gfxFactory->CreateVertexBuffer(sizeof(vertices), sizeof(vertices[0]));
		m_vertexBuffer->Copy(vertices);

		m_indexBuffer = gfxFactory->CreateIndexBuffer(sizeof(indices), sizeof(indices[0]));
		m_indexBuffer->Copy(indices);

		OnInit(texture, w, h);
	}
	void ISprite::Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot)
	{
		//�s�{�b�g���l���ɓ��ꂽ���s�ړ��s����쐬�B
		//�s�{�b�g�͐^�񒆂�0.0, 0.0�A���オ-1.0f, -1.0�A�E����1.0�A1.0�ɂȂ�悤�ɂ���B
		CVector2 localPivot = pivot;
		localPivot.x -= 0.5f;
		localPivot.y -= 0.5f;
		localPivot.x *= -2.0f;
		localPivot.y *= -2.0f;
		//�摜�̃n�[�t�T�C�Y�����߂�B
		CVector2 halfSize = m_size;
		halfSize.x *= 0.5f;
		halfSize.y *= 0.5f;
		CMatrix mPivotTrans;

		mPivotTrans.MakeTranslation(
			{ halfSize.x * localPivot.x, halfSize.y * localPivot.y, 0.0f }
		);
		CMatrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);
		m_world = mPivotTrans * mScale;
		m_world = m_world * mRot;
		m_world = m_world * mTrans;
		OnUpdate(pos, rot, scale, pivot);
	}
	void ISprite::Draw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix)
	{
		OnDraw(renderContext, viewMatrix, projMatrix);
	}

}