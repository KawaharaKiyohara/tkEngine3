/// <summary>
/// tkEngine�̃O���t�B�b�N�֘A�̃w�b�_�[�B
/// </summary>

/// <summary>
/// �v���~�e�B�u�g�|���W�[�B
/// </summary>
enum EnPrimitiveTopology {
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
	enPrimitiveTopology_TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
#endif
	enPrimitiveTopology_Undex = 0xFFFFFFFF
};

