#include "tkEngine/tkEnginePreCompile.h"
#include "tkEngine/graphics/Dx12/dx12Common.h"
#include "tkTonemapDx12.h"

namespace tkEngine {
	namespace {

		//-----------------------------------------------------------------------------
		// Name: GetSampleOffsets_DownScale4x4
		// Desc: Get the texture coordinate offsets to be used inside the DownScale4x4
		//       pixel shader.
		//-----------------------------------------------------------------------------
		HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, CVector4 avSampleOffsets[])
		{
			if (NULL == avSampleOffsets)
				return E_INVALIDARG;

			float tU = 1.0f / dwWidth;
			float tV = 1.0f / dwHeight;

			// Sample from the 16 surrounding points. Since the center point will be in
			// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
			// center.
			int index = 0;
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 4; x++)
				{
					avSampleOffsets[index].x = (x - 1.5f) * tU;
					avSampleOffsets[index].y = (y - 1.5f) * tV;

					index++;
				}
			}

			return S_OK;
		}
	}
	void CTonemapDx12::Init(const SGraphicsConfig& config)
	{
		if (!config.tonemapConfig.isEnable) {
			return;
		}
		m_isEnable = true;
		//���ϋP�x�v�Z�p�̃����_�����O�^�[�Q�b�g���쐬�B
		for (int i = 0; i < NUM_CALC_AVG_RT; i++)
		{
			int rtSize = 1 << (2 * i);
			m_calcAvgRT[i].Create(
				rtSize,
				rtSize,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}
		//�ŏI�I�Ƀg�[���}�b�v�Ŏg�p���镽�ϋP�x���������ރ����_�����O�^�[�Q�b�g���쐬�B
		for (auto& rt : m_avgRT) {
			rt.Create(
				1,
				1,
				1,
				1,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				DXGI_FORMAT_UNKNOWN
			);
		}
		//���[�g�V�O�l�`���̏������B
		m_rootSignature.Init(
			D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			D3D12_TEXTURE_ADDRESS_MODE_CLAMP
		);
		//�萔�o�b�t�@�̏������B
		m_tonemapParam.middleGray = config.tonemapConfig.luminance;
		m_cbTonemapCommon.Init(sizeof(m_tonemapParam) , &m_tonemapParam);
		//�V�F�[�_�[���������B
		m_vs.LoadVS(L"shader/tonemap.fx", "VSMain");
		m_psCalcLuminanceLogAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceLogAvarage");
		m_psCalcLuminanceAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceAvarage");
		m_psCalcLuminanceExpAvarageShader.LoadPS(L"shader/tonemap.fx", "PSCalcLuminanceExpAvarage");
		m_psCalcAdaptedLuminanceShader.LoadPS(L"shader/tonemap.fx", "PSCalcAdaptedLuminance");
		m_psCalcAdaptedLuminanceFirstShader.LoadPS(L"shader/tonemap.fx", "PSCalcAdaptedLuminanceFirst");
		m_psFinal.LoadPS(L"shader/tonemap.fx", "PSFinal");

		for (auto& cb : m_cbCalcLuminance) {
			cb.Init(sizeof(m_avSampleOffsets), nullptr);
		}

		//�p�C�v���C���X�e�[�g���������B
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
		psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_rootSignature.Get();
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.GetCompiledBlob().Get());
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceLogAvarageShader.GetCompiledBlob().Get());
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
		m_calcLuminanceLogAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceAvarageShader.GetCompiledBlob().Get());
		m_calsLuminanceAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcLuminanceExpAvarageShader.GetCompiledBlob().Get());
		m_calsLuminanceExpAvaragePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminanceFirstShader.GetCompiledBlob().Get());
		m_psCalcAdaptedLuminanceFirstPipelineState.Init(psoDesc);

		psoDesc.PS  = CD3DX12_SHADER_BYTECODE(m_psCalcAdaptedLuminanceFirstShader.GetCompiledBlob().Get());
		m_psCalcAdaptedLuminancePipelineState.Init(psoDesc);

		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_psFinal.GetCompiledBlob().Get());
		m_finalPipelineState.Init(psoDesc);

		//�f�B�X�N���v�^�q�[�v���쐬�B
		CreateDescriptorHeap();
	}
	void CTonemapDx12::CreateDescriptorHeap()
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		int curRtNo = NUM_CALC_AVG_RT - 1;
		{
			m_calcAvgDescriptorHeap[curRtNo].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			m_calcAvgDescriptorHeap[curRtNo].RegistConstantBuffer(1, m_cbCalcLuminance[curRtNo]);
			m_calcAvgDescriptorHeap[curRtNo].RegistShaderResource(0, ge12.GetMainRenderTarget().GetRenderTargetTexture()); (1, m_cbCalcLuminance[curRtNo]);
			m_calcAvgDescriptorHeap[curRtNo].Commit();
		}
		curRtNo--;
		{
			while (curRtNo > 0) {
				m_calcAvgDescriptorHeap[curRtNo].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				m_calcAvgDescriptorHeap[curRtNo].RegistConstantBuffer(1, m_cbCalcLuminance[curRtNo]);
				m_calcAvgDescriptorHeap[curRtNo].RegistShaderResource(0, m_calcAvgRT[curRtNo + 1].GetRenderTargetTexture()); (1, m_cbCalcLuminance[curRtNo]);
				m_calcAvgDescriptorHeap[curRtNo].Commit();
				curRtNo--;
			}
		}
		m_calcAvgDescriptorHeap[curRtNo].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_calcAvgDescriptorHeap[curRtNo].RegistConstantBuffer(1, m_cbCalcLuminance[curRtNo]);
		m_calcAvgDescriptorHeap[curRtNo].RegistShaderResource(0, m_calcAvgRT[curRtNo + 1].GetRenderTargetTexture());
		m_calcAvgDescriptorHeap[curRtNo].Commit();

		//���Ï����B
		m_lightDarkAdaptationFirstDS.Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_lightDarkAdaptationFirstDS.RegistShaderResource(0, m_calcAvgRT[0].GetRenderTargetTexture());
		m_lightDarkAdaptationFirstDS.Commit();

		m_lightDarkAdaptation[0].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_lightDarkAdaptation[0].RegistConstantBuffer(0, m_cbTonemapCommon);
		m_lightDarkAdaptation[0].RegistShaderResource(1, m_calcAvgRT[0].GetRenderTargetTexture());
		m_lightDarkAdaptation[0].RegistShaderResource(2, m_avgRT[1].GetRenderTargetTexture()); //0�̂Ƃ��͂P
		m_lightDarkAdaptation[0].Commit();

		m_lightDarkAdaptation[1].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_lightDarkAdaptation[1].RegistConstantBuffer(0, m_cbTonemapCommon);
		m_lightDarkAdaptation[1].RegistShaderResource(1, m_calcAvgRT[0].GetRenderTargetTexture());
		m_lightDarkAdaptation[1].RegistShaderResource(2, m_avgRT[0].GetRenderTargetTexture()); //1�̂Ƃ���0
		m_lightDarkAdaptation[1].Commit();

		//�ŏI�����p�̃f�B�X�N���v�^�q�[�v�B
		m_finalCombineDS[0].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_finalCombineDS[0].RegistConstantBuffer(0, m_cbTonemapCommon);
		m_finalCombineDS[0].RegistShaderResource(0, ge12.GetMainRenderTarget().GetRenderTargetTexture());
		m_finalCombineDS[0].RegistShaderResource(1, m_avgRT[0].GetRenderTargetTexture());
		m_finalCombineDS[0].Commit();

		m_finalCombineDS[1].Init(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		m_finalCombineDS[1].RegistConstantBuffer(0, m_cbTonemapCommon);
		m_finalCombineDS[1].RegistShaderResource(0, ge12.GetMainRenderTarget().GetRenderTargetTexture());
		m_finalCombineDS[1].RegistShaderResource(1, m_avgRT[0].GetRenderTargetTexture());
		m_finalCombineDS[1].Commit();

	}
	void CTonemapDx12::CalcLuminanceAvarage(CRenderContextDx12& rc)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//	
		rc.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
		//�ΐ����ς����߂�B
		float tU, tV;
		tU = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetWidth());
		tV = 1.0f / (3.0f * m_calcAvgRT[NUM_CALC_AVG_RT - 1].GetHeight());

		int index = 0;
		for (int x = -1; x <= 1; x++)
		{
			for (int y = -1; y <= 1; y++)
			{
				m_avSampleOffsets[index].x = x * tU;
				m_avSampleOffsets[index].y = y * tV;

				index++;
			}
		}
		int curRtNo = NUM_CALC_AVG_RT - 1;
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);

			m_cbCalcLuminance[curRtNo].Update(m_avSampleOffsets);
			rc.SetPipelineState(m_calcLuminanceLogAvaragePipelineState);
			//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
			rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);			
			rc.DrawIndexedFast(4);
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}
		//��������̓_�E���T���v�����O�ŋ��߂�B
		curRtNo--;
		{
			while (curRtNo > 0) {
				rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
				rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);
				GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
				m_cbCalcLuminance[curRtNo].Update(&m_avSampleOffsets);
				//�f�B�X�N���v�^�q�[�v���Z�b�g�B
				rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
				rc.SetPipelineState(m_calsLuminanceAvaragePipelineState);
				rc.DrawIndexedFast(4);
				rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
				curRtNo--;
			}
		}
		//�Ō��exp�֐���p���čŏI���ς����߂�B
		{
			rc.WaitUntilToPossibleSetRenderTarget(m_calcAvgRT[curRtNo]);
			rc.SetRenderTargetAndViewport(m_calcAvgRT[curRtNo]);
			GetSampleOffsets_DownScale4x4(m_calcAvgRT[curRtNo].GetWidth(), m_calcAvgRT[curRtNo].GetHeight(), m_avSampleOffsets);
			m_cbCalcLuminance[curRtNo].Update(&m_avSampleOffsets);
			//�V�F�[�_�[���\�[�X�r���[�ƒ萔�o�b�t�@���Z�b�g����B
			rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
			
			//�p�C�v���C���X�e�[�g��ݒ�B
			rc.SetPipelineState(m_calsLuminanceExpAvaragePipelineState);
			rc.DrawIndexedFast(4);
			rc.WaitUntilFinishDrawingToRenderTarget(m_calcAvgRT[curRtNo]);
		}

		//���Ï����B
		{
			if (m_isFirstWhenChangeScene == true) {
				//�V�[�����؂�ւ���ď���B
				m_currentAvgRt = 1 ^ m_currentAvgRt;
				rc.WaitUntilToPossibleSetRenderTarget(m_avgRT[m_currentAvgRt]);

				rc.SetRenderTargetAndViewport(m_avgRT[m_currentAvgRt]);		
				//�f�B�X�N���v�q�[�v��ݒ肷��B
				rc.SetDescriptorHeap(m_calcAvgDescriptorHeap[curRtNo]);
				rc.DrawIndexedFast(4);
				m_isFirstWhenChangeScene = false;
			}
			else {

				auto& lastRT = m_avgRT[m_currentAvgRt];
				m_currentAvgRt = 1 ^ m_currentAvgRt;

				rc.WaitUntilToPossibleSetRenderTarget(m_avgRT[m_currentAvgRt]);
				rc.SetRenderTargetAndViewport(m_avgRT[m_currentAvgRt]);

				rc.SetDescriptorHeap(m_lightDarkAdaptation[m_currentAvgRt]);
				
				rc.SetPipelineState(m_psCalcAdaptedLuminancePipelineState);
				rc.DrawIndexedFast(4);
			}
			rc.WaitUntilFinishDrawingToRenderTarget(m_avgRT[m_currentAvgRt]);
		}
	}
	void CTonemapDx12::Render(IRenderContext& rc)
	{
		if (!m_isEnable) {
			return;
		}
		auto& rc12 = rc.As<CRenderContextDx12>();
		rc12.SetRootSignature(m_rootSignature);
		m_tonemapParam.deltaTime = g_gameTime->GetFrameDeltaTime();
		m_cbTonemapCommon.Update(&m_tonemapParam);
		//���ϋP�x�̌v�Z�B
		CalcLuminanceAvarage(rc12);

		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//	
		//���ϋP�x����g�[���}�b�v���s���B
		rc12.WaitUntilToPossibleSetRenderTarget(ge12.GetMainRenderTarget());
		rc12.SetRenderTargetAndViewport(ge12.GetMainRenderTarget());
		
		rc12.SetDescriptorHeap(m_finalCombineDS[m_currentAvgRt]);

		rc12.SetPipelineState(m_finalPipelineState);
		rc12.DrawIndexedFast(4);

		rc12.WaitUntilFinishDrawingToRenderTarget(ge12.GetMainRenderTarget());
	}
}