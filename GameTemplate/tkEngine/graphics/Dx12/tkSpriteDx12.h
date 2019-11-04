#pragma once
#include "tkEngine/Graphics/tkSprite.h"
#include "tkRootSignatureDx12.h"
#include "tkShaderDx12.h"
#include "tkConstantBufferDx12.h"

namespace tkEngine {
	class CSpriteDx12 : public ISprite {
	public:
		~CSpriteDx12();
	private:
		
		/// <summary>
		/// ���������ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="texture">�e�N�X�`��</param>
		/// <param name="w">��</param>
		/// <param name="h">����</param>
		void OnInit(ITexture* texture, float w, float h) override final;
	
		/// <summary>
		/// �X�V���ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="rot">��]</param>
		/// <param name="scale">�g�嗦</param>
		/// <param name="pivot">
		/// �s�{�b�g
		/// 0.5, 0.5�ŉ摜�̒��S����_�B
		/// 0.0, 0.0�ŉ摜�̍����B
		/// 1.0, 1.0�ŉ摜�̉E��B
		/// Unity��uGUI�ɏ����B
		/// </param>
		void OnUpdate(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot) override final;
		/// <summary>
		/// �`�掞�ɌĂ΂�鏈���B�h���N���X�Ŏ������Ă��������B
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		/// <param name="viewMatrix">�r���[�s��</param>
		/// <param name="projMatrix">�v���W�F�N�V�����s��</param>
		void OnDraw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix) override final;

		void InitPipelineState();
		void InitDescriptorHeap();
	private:
		struct SConstantBuffer {
			CMatrix mvp;
			CVector4 mulColor;
		};
		CRootSignatureDx12 m_rootSignature;	//���[�g�V�O�l�`���B
		ComPtr< ID3D12PipelineState>	m_pipelineState;	//�p�C�v���C���X�e�[�g�B
		CShaderDx12 m_vs;								//���_�V�F�[�_�[�B
		CShaderDx12 m_ps;								//�s�N�Z���V�F�[�_�[�B
		SConstantBuffer m_constantBufferCPU;			//CPU���̒萔�o�b�t�@�B
		CConstantBufferDx12 m_constantBufferGPU;		//GPU���̒萔�o�b�t�@�B
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap;	//�f�B�X�N���v�^�q�[�v�B
	};
}