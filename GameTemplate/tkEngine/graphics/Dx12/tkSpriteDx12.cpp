#include "tkEngine/tkEnginePreCompile.h"
#include "dx12Common.h"
#include "tkSpriteDx12.h"

namespace tkEngine {
	CSpriteDx12::~CSpriteDx12()
	{
	}
	void CSpriteDx12::OnInit(ITexture* texture, float w, float h)
	{
		//���[�g�V�O�l�`���̏������B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP);
		//�V�F�[�_�[�����[�h�B
		m_vs.Load(L"shader/sprite.fx", "VSMain", "vs_5_0");
		m_ps.Load(L"shader/sprite.fx", "PSMain", "ps_5_0");
		//�p�C�v���C���X�e�[�g���������B
		InitPipelineState();
		//�f�B�X�N���v�^�q�[�v���������B
		InitDescriptorHeap();
		//�萔�o�b�t�@�̍쐬�B
		m_constantBufferGPU.Init(sizeof(m_constantBufferCPU), nullptr);
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
		//���[�g�V�O�l�`����ݒ�B
		rc12.SetRootSignature(m_rootSignature);
		//�p�C�v���C���X�e�[�g��ݒ�B
		rc12.SetPipelineState(m_pipelineState);
		//���_�o�b�t�@��ݒ�B
		rc12.SetVertexBuffer(m_vertexBuffer);
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		rc12.SetIndexBuffer(m_indexBuffer);
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		rc12.SetPrimitiveTopology(enPrimitiveTopology_TriangleStrip);

		//�萔�o�b�t�@�ƃV�F�[�_�[���\�[�X��ݒ�B
		auto& texture = m_texture->As<CTextureDx12 >();
		IShaderResourceDx12* srvTbl[] = {
			&texture
		};
		CConstantBufferDx12* cbrTbl[] = {
			&m_constantBufferGPU
		};
		rc12.SetCBR_SRV_UAV(
			m_descriptorHeap.Get(),
			ARRAYSIZE(cbrTbl),
			cbrTbl,
			ARRAYSIZE(srvTbl),
			srvTbl);

		//�h���[�B
		rc12.DrawIndexed(m_indexBuffer->GetCount());
	}
	void CSpriteDx12::InitPipelineState()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();
		// ���_���C�A�E�g���`����B
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//�p�C�v���C���X�e�[�g���쐬�B
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.GetCompiledBlob().Get());
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.SampleDesc.Count = 1;
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));

	}
	void CSpriteDx12::InitDescriptorHeap()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 8;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_descriptorHeap));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");

	}

}