#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/tkPrimitive.h"

namespace tkEngine {
	void CPrimitive::Init(
		void* srcVertexData,
		int vertexBufferSize,
		int vertexBufferStride,
		void* srcIndexData,
		int indexBufferSize,
		int indexBufferStride,
		EnPrimitiveTopology enPrimTopology)
	{
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬�B
		auto factory = Engine().GetGraphicsInstanceFactory();
		m_vertexBuffer = factory->CreateVertexBuffer(vertexBufferSize, vertexBufferStride);
		m_indexBuffer = factory->CreateIndexBuffer(indexBufferSize, indexBufferStride);
		m_vertexBuffer->Copy(srcVertexData);
		m_indexBuffer->Copy(srcIndexData);
		m_topology = enPrimTopology;
	}
}