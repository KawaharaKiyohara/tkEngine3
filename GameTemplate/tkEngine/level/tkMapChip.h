/*!
 *@brief	�}�b�v�`�b�v
 */

#pragma once

//#include "tkEngine/physics/tkPhysicsStaticObject.h"
#include "tkEngine/level/tkMapChipRender.h"

namespace tkEngine{
	struct LevelObjectData;
	class CMapChipRender;
	/*!
	 *@brief	�}�b�v�`�b�v�B
	 */
	class CMapChip : Noncopyable{
	public:
		CMapChip(const LevelObjectData& objData, CMapChipRender* mapChipRender);
		~CMapChip()
		{
		}
		/*!
		*@brief	���f�������_���[���擾�B
		*/
		prefab::CModelRender* GetSkinModelRender()
		{
			if (m_mapChipRender == nullptr ){
				return nullptr;
			}
			return m_mapChipRender->GetModelRender();
		}
		
	private:
		CMapChipRender* m_mapChipRender = nullptr;			//�}�b�v�`�b�v�����_���[�B
		//CPhysicsStaticObject m_physicsStaticObject;			//!<�ÓI�����I�u�W�F�N�g�B
	};
}