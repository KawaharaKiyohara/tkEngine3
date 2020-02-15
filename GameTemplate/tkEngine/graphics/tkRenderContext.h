#pragma once

namespace tkEngine {
	/// <summary>
	/// �����_�����O�p�X�B
	/// </summary>
	enum EnRenderStep {
		enRenderStep_Undef,							//����`
		enRenderStep_CreateDirectionalShadowMap,	//�w�����V���h�E�}�b�v�̍쐬�B
		enRenderStep_ForwardRender,					//�t�H���[�h�����_�����O�B
	};
	class IRenderContext : Noncopyable {
	public:
		
		virtual ~IRenderContext() {}
		/// <summary>
		/// IRenderContext�̃C���X�^���X�̌^�ϊ��B
		/// </summary>
		template<class T>
		T& As()
		{
			return dynamic_cast<T&>(*this);
		}
		/// <summary>
		/// �����_�����O�X�e�b�v��ݒ�B
		/// </summary>
		/// <param name="renderStep"></param>
		void SetRenderStep(EnRenderStep renderStep)
		{
			m_renderStep = renderStep;
		}
		/// <summary>
		/// ���݂̃����_�����O�X�e�b�v���擾�B
		/// </summary>
		/// <returns></returns>
		EnRenderStep GetRenderStep() const
		{
			return m_renderStep;
		}
	private:
		EnRenderStep m_renderStep = enRenderStep_Undef;
	};
	using UPIRenderContext = std::unique_ptr<IRenderContext>;
}
