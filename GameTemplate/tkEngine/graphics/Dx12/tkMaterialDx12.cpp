#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMaterialDx12.h"

namespace tkEngine {

	void CMaterialDx12::InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat)
	{
		//todo �V�F�[�_�[�͉��B
		m_vs.Load(L"shader/Primitive.fx", "VSMainNoTexture", "vs_5_0");
		m_ps.Load(L"shader/Primitive.fx", "PSMainNoTexture", "ps_5_0");
		//todo	�e�N�X�`���͂܂��A�Z�b�g�p�C�v���C�����ł��Ă��Ȃ��̂Ō�񂵁B
		//�e�N�X�`�������[�h�B
		//mat->m_albedoMap.InitFromDDSFile
		//todo ���̓p�C�v���C���X�e�[�g���쐬�B
		//�����Q��B
	}
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12