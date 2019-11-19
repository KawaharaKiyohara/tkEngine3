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
		//���[�g�V�O�l�`�����쐬�B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER);

		//�V�F�[�_�[���������B
		InitShaders();
		//�����_�����O�^�[�Q�b�g���������B
		InitRenderTargets();
		//�l�p�`�v���~�e�B�u���������B
		InitQuadPrimitive();
		//�p�C�v���C���X�e�[�g���������B
		InitPipelineState();
		
		for (auto& cb : m_blurParamCB) {
			//�萔�o�b�t�@���������B
			cb.Init(sizeof(m_blurParam), nullptr);
		}

		
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
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN);

			TK_ASSERT(result, "���u���[�p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");

			//�c�u���[�p�B
			result = m_downSamplingRT[baseIndex + 1].Create(
				w >> shift,
				h >> shift,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN);
			TK_ASSERT(result, "�c�u���[�p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");

		}
		//�{�P�摜�����p�̃����_�����O�^�[�Q�b�g���쐬�B
		result = m_combineRT.Create( w >> 2, h >> 2, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN );
		TK_ASSERT(result, "�{�P�摜�����p�̃����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B");
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
		psoDesc.pRootSignature = m_rootSignature.Get(); 
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

		//X�u���[�p�̃p�C�v���C���X�e�[�g�B
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsXBlur.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_xblurLuminancePipelineState));

		//y�u���[�p�̃p�C�v���C���X�e�[�g�B
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vsYBlur.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psBlur.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_yblurLuminancePipelineState));
		//�{�P�摜�����p�̃p�C�v���C���X�e�[�g�B
		psoDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCombine.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_combineBokeImagePipelineState));

		//�ŏI�����p�̃p�C�v���C���X�e�[�g�B
		psoDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
		psoDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		psoDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		psoDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_copyVS.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_copyPS.GetCompiledBlob().Get());
		d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_combineMainRenderTargetPipelineState));

	}

	void CBloomDx12::UpdateWeight(float dispersion)
	{
		float total = 0;
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] = expf(-0.5f * (float)(i * i) / dispersion);
			total += 2.0f * m_blurParam.weights[i];

		}
		// �K�i��
		for (int i = 0; i < NUM_WEIGHTS; i++) {
			m_blurParam.weights[i] /= total;
		}
	}
	void CBloomDx12::SamplingLuminance(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		//���C�������_�����O�^�[�Q�b�g���e�N�X�`���Ƃ��ė��p����̂�
		//���p�ł���悤�ɂȂ�܂őҋ@����B
		rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
		rc12.WaitUntilToPossibleSetRenderTarget(m_luminanceRT);
		//�p�C�v���C���X�e�[�g��ݒ�B
		rc12.SetPipelineState(m_samplingLuminancePipelineState);
		//�����_�����O�^�[�Q�b�g���P�x���o�p�ɐ؂�ւ���B
		rc12.SetRenderTarget(m_luminanceRT);
		const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		rc12.ClearRenderTargetView(m_luminanceRT, clearColor);
		//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
		IShaderResourceDx12* srvTbl[] = {
			&ge12.GetMainRenderTarget().GetRenderTargetTexture()
		};
		rc12.SetCBR_SRV_UAV(
			0,
			nullptr,
			1,
			srvTbl
		);
		//�h���h���B
		rc12.DrawIndexed(4);
		rc12.WaitUntilFinishDrawingToRenderTarget(m_luminanceRT);	
	}
	void CBloomDx12::BlurLuminanceTexture(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		CRenderTargetDx12* prevRt = &m_luminanceRT;

		int rtIndex = 0;
		for (int i = 0; i < NUM_DOWN_SAMPLING_RT / 2; i++) {
			//X�u���[�B
			{
				m_blurParam.offset.x = 16.0f / prevRt->GetWidth();
				m_blurParam.offset.y = 0.0f;
				m_blurParamCB[rtIndex].Update(&m_blurParam);

				rc12.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				rc12.SetPipelineState(m_xblurLuminancePipelineState);
				rc12.SetRenderTargetAndViewport(m_downSamplingRT[rtIndex]);
				
				//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
				IShaderResourceDx12* srvTbl[] = {
					&prevRt->GetRenderTargetTexture()
				};
				CConstantBufferDx12* cbrTbl[] = {
					&m_blurParamCB[rtIndex]
				};
				rc12.SetCBR_SRV_UAV(
					1,
					cbrTbl,
					1,
					srvTbl
				);
				rc12.DrawIndexed(4);
				rc12.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
			//Y�u���[�B
			{
				m_blurParam.offset.x = 0.0f;
				m_blurParam.offset.y = 16.0f / prevRt->GetWidth();
				m_blurParamCB[rtIndex].Update(&m_blurParam);

				rc12.WaitUntilToPossibleSetRenderTarget(m_downSamplingRT[rtIndex]);
				rc12.SetRenderTargetAndViewport(m_downSamplingRT[rtIndex]);
				rc12.SetPipelineState(m_yblurLuminancePipelineState);

				//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
				IShaderResourceDx12* srvTbl[] = {
					&prevRt->GetRenderTargetTexture()
				};
				CConstantBufferDx12* cbrTbl[] = {
					&m_blurParamCB[rtIndex]
				};
				rc12.SetCBR_SRV_UAV(
					1,
					cbrTbl,
					1,
					srvTbl
				);
				rc12.DrawIndexed(4);
				rc12.WaitUntilFinishDrawingToRenderTarget(m_downSamplingRT[rtIndex]);
			}
			prevRt = &m_downSamplingRT[rtIndex];
			rtIndex++;
		}
	}
	void CBloomDx12::CombineBokeImage(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		rc12.WaitUntilToPossibleSetRenderTarget(m_combineRT);
		rc12.SetPipelineState(m_combineBokeImagePipelineState);
		rc12.SetRenderTargetAndViewport(m_combineRT);

		//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
		IShaderResourceDx12* srvTbl[] = {
			&m_downSamplingRT[3].GetRenderTargetTexture(),
			&m_downSamplingRT[5].GetRenderTargetTexture(),
			&m_downSamplingRT[7].GetRenderTargetTexture(),
			&m_downSamplingRT[9].GetRenderTargetTexture(),
		};
	
		rc12.SetCBR_SRV_UAV(
			0,
			nullptr,
			4,
			srvTbl
		);
		rc12.DrawIndexed(4);
		rc12.WaitUntilFinishDrawingToRenderTarget(m_combineRT);

	}
	void CBloomDx12::CombineMainRenderTarget(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12)
	{
		rc12.WaitUntilToPossibleSetRenderTarget(ge12.GetMainRenderTarget());
		rc12.SetPipelineState(m_combineMainRenderTargetPipelineState);
		rc12.SetRenderTargetAndViewport(ge12.GetMainRenderTarget());

		//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
		IShaderResourceDx12* srvTbl[] = {&m_combineRT.GetRenderTargetTexture()};

		rc12.SetCBR_SRV_UAV(
			0,
			nullptr,
			1,
			srvTbl
		);
		rc12.DrawIndexed(4);
	//	rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
	}
	void CBloomDx12::Render(IRenderContext& rc)
	{
		auto& rc12 = rc.As<CRenderContextDx12>();
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();

		//�d�݂��X�V�B
		UpdateWeight(25.0f);

		//���[�g�V�O�l�`����ݒ�B
		rc12.SetRootSignature(m_rootSignature);
		//���_�o�b�t�@��ݒ�B
		rc12.SetVertexBuffer(m_quadPrimitive.GetVertexBuffer());
		//�C���f�b�N�X�o�b�t�@��ݒ�B
		rc12.SetIndexBuffer(m_quadPrimitive.GetIndexBuffer());
		//�v���~�e�B�u�g�|���W�[��ݒ肷��B
		rc12.SetPrimitiveTopology(m_quadPrimitive.GetPrimitiveTopology());

		//�P�x�𒊏o����
		SamplingLuminance(ge12, rc12);
		//�P�x�e�N�X�`�����ڂ����B
		BlurLuminanceTexture(ge12, rc12);
		//�{�P�摜����������B
		CombineBokeImage(ge12, rc12);
		//�ŏI����
		CombineMainRenderTarget(ge12, rc12);
	}
}