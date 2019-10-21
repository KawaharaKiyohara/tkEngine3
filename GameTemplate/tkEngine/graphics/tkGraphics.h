/// <summary>
/// tkEngine�̃O���t�B�b�N�֘A�̃w�b�_�[�B
/// </summary>



namespace tkEngine {
	/// <summary>
	/// �v���~�e�B�u�g�|���W�[�B
	/// </summary>
	enum EnPrimitiveTopology {
#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
		enPrimitiveTopology_TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
#endif
		enPrimitiveTopology_Undex = 0xFFFFFFFF
	};

	/// <summary>
	/// �O���t�B�b�N�X�n�̃��j�[�N�|�C���^
	/// </summary>
	/// <remarks>
	/// �����͒��_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�e�N�X�`���Ȃǂ�
	/// �A�N�Z�X����C���^�[�t�F�[�X�N���X�ł��B
	/// �����̃C���X�^���X��CEngine�N���X���ێ����Ă���Am_graphisInstanceFactory��
	/// ���p���邱�Ƃō쐬����܂��B
	/// </remarks>
	class IShape;
	class IVertexBuffer;
	class IIndexBuffer;
	class IGraphicsEngine;
	class IGraphicsInstanceFactory;
	class ITexture;
	class IMeshParts;
	class IRenderContext;
	class ILightManager;

	using UPIGraphicsEngine = std::unique_ptr< IGraphicsEngine>;
	using UPIVertexBuffer = std::unique_ptr < IVertexBuffer >;
	using UPIIndexBuffer = std::unique_ptr<IIndexBuffer>;
	using UPIRenderContext = std::unique_ptr<IRenderContext>;
	using UPIGraphicsInstanceFactory = std::unique_ptr< IGraphicsInstanceFactory>;
	using UPIShape = std::unique_ptr<IShape>;
	using UPITexture = std::unique_ptr<ITexture>;
	using UPIMeshParts = std::unique_ptr<IMeshParts>;
	using UPILightManager = std::unique_ptr< ILightManager >;
}

