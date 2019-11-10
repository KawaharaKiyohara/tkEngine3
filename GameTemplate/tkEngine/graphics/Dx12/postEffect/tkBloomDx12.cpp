#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkBloomDx12.h"

namespace tkEngine {
	namespace {
		struct SSimpleVertex {
			CVector4 pos;
			CVector2 tex;
		};
	}
	void CBloomDx12::Init(const SGraphicsConfig& config)
	{
		if (!config.bloomConfig.isEnable) {
			return;
		}

		//�V�F�[�_�[���������B
		InitShaders();
		//�����_�����O�^�[�Q�b�g���������B
		InitRenderTargets();
		//�l�p�`�v���~�e�B�u���������B
		InitQuadPrimitive();
		//�p�C�v���C���X�e�[�g���������B
		InitPipelineState();
		//�f�B�X�N���v�^�q�[�v�̏������B
		InitDescriptorHeap();
	}
	void CBloomDx12::InitRenderTargets()
	{
		int w = g_graphicsEngine->GetFrameBufferWidth();
		int h = g_graphicsEngine->GetFrameBufferHeight();
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬�B
		bool result = m_luminanceRT.Create(w, h, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
		TK_ASSERT(result, "�P�x���o�p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");

		//�_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g���쐬����B
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
			int shift = i + 1;
			int baseIndex = i * 2;
			//���u���[�p�B
			result = m_downSamplingRT[baseIndex].Create(
				w >> shift,
				h >> (shift - 1),
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN);

			TK_ASSERT(result, "���u���[�p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");

			//�c�u���[�p�B
			result = m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R16G16B16A16_FLOAT,
				DXGI_FORMAT_UNKNOWN);
			TK_ASSERT(result, "�c�u���[�p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");

		}
	}
	void CBloomDx12::InitQuadPrimitive()
	{
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
	void CBloomDx12::InitShaders()
	{
		//�V�F�[�_�[�����[�h�B
		m_vs.Load(L"shader/bloom.fx", "VSMain", g_vsShaderModelName);
		m_psLuminance.Load(L"shader/bloom.fx", "PSSamplingLuminance", g_psShaderModelName);
		m_vsXBlur.Load(L"shader/bloom.fx", "VSXBlur", g_vsShaderModelName);
		m_vsYBlur.Load(L"shader/bloom.fx", "VSYBlur", g_vsShaderModelName);
		m_psBlur.Load(L"shader/bloom.fx", "PSBlur", g_psShaderModelName);
		m_psCombine.Load(L"shader/bloom.fx", "PSCombine", g_psShaderModelName);
		m_copyVS.Load(L"shader/copy.fx", "VSMain", g_vsShaderModelName);
		m_copyPS.Load(L"shader/copy.fx", "PSMain", g_psShaderModelName);

	}
	void CBloomDx12::InitPipelineState()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		//�p�C�v���C���X�e�[�g���쐬�B
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = CPipelineStatesDx12::m_modelDrawRootSignature.Get(); //���f���`��Ƌ��ʂł����ˁ[��B
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psLuminance.GetCompiledBlob().Get());
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
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
		psoDesc.SampleDesc.Count = 1;
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_samplingLuminancePipelineState));

	}
	void CBloomDx12::InitDescriptorHeap()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto d3dDevice = ge12.GetD3DDevice();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_luminanceDescriptorHeap));
		TK_ASSERT(SUCCEEDED(hr), "CSpriteDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");

	}
	void CBloomDx12::SamplingLuminance(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		//���C�������_�����O�^�[�Q�b�g���e�N�X�`���Ƃ��ė��p����̂�
		//���p�ł���悤�ɂȂ�܂őҋ@����B
		rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
		//�p�C�v���C���X�e�[�g��ݒ�B
		rc12.SetPipelineState(m_samplingLuminancePipelineState);
		//�����_�����O�^�[�Q�b�g���P�x���o�p�ɐ؂�ւ���B
		rc12.SetRenderTarget(m_luminanceRT);
		//���_�o�b�t�@��ݒ�B
		rc12.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		rc12.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		rc12.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());
		//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
		IShaderResourceDx12* srvTbl[] = {
			&ge12.GetMainRenderTarget().GetRenderTargetTexture()
		};
		rc12.SetCBR_SRV_UAV(
			m_luminanceDescriptorHeap.Get(),
			0,
			nullptr,
			1,
			srvTbl
		);
		//�h���h���B
		rc12.DrawIndexed(4);

		
	}
	void CBloomDx12::Render(IRenderContext& rc)
	{
		auto& rc12 = rc.As<CRenderContextDx12>();
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//�P�x�𒊏o����
		SamplingLuminance(ge12, rc12);

		rc12.WaitUntilToPossibleSetRenderTarget(ge12.GetMainRenderTarget());
		//�����_�����O�^�[�Q�b�g���P�x���o�p�ɐ؂�ւ���B
		rc12.SetRenderTarget(ge12.GetMainRenderTarget());
	}
}