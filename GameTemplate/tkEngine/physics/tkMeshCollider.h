#pragma once
#include "tkEngine/Physics/tkICollider.h"

namespace tkEngine{

	/*!
	 * @brief	���b�V���R���C�_�[�B
	 */
	class CMeshCollider : public ICollider
	{
	public:
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		CMeshCollider();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~CMeshCollider();
		/*!
		 * @brief	CModel���烁�b�V���R���C�_�[�𐶐��B
		 *@param[in]	model		�X�L�����f���B
		 */
		void CreateFromModel( const CModel& model, const CMatrix* offsetMatrix );
		/*!
		* @brief	tkm�t�@�C�����烁�b�V���R���C�_�[�𐶐��B
		*@param[in]	tkmFile		tkm�t�@�C���B
		*/
		void CreateFromTkmFile(const CTkmFile& tkmFile, const CMatrix* offsetMatrix);
		btCollisionShape* GetBody() const override
		{
			return m_meshShape.get();
		}
	private:
		typedef std::vector<CVector3>		VertexBuffer;		//���_�o�b�t�@�B
		typedef std::vector<unsigned int>	IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
		typedef std::unique_ptr<VertexBuffer>	VertexBufferPtr;
		typedef std::unique_ptr<IndexBuffer>	IndexBufferPtr;
		std::vector<VertexBufferPtr>					m_vertexBufferArray;		//���_�o�b�t�@�̔z��B
		std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
		std::unique_ptr<btBvhTriangleMeshShape>			m_meshShape;				//���b�V���`��B
		std::unique_ptr<btTriangleIndexVertexArray>		m_stridingMeshInterface;
	};
}
