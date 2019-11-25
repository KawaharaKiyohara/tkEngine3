#pragma once

namespace tkEngine {
	/// <summary>
	/// トーンマップ。
	/// </summary>
	class CTonemapDx12 : Noncopyable {
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="config">グラフィック設定</param>
		void Init(const SGraphicsConfig& config);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render(IRenderContext& rc);
	public:
		static const int MAX_SAMPLES = 16;
	private:
		struct STonemapParam {
			float deltaTime;
			float middleGray;
		};
		static const int NUM_CALC_AVG_RT = 5;							//平均輝度計算用のレンダリングターゲットの枚数。
		CRenderTargetDx12 m_calcAvgRT[NUM_CALC_AVG_RT];	//平均輝度計算用のレンダリングターゲット。
		CRenderTargetDx12 m_avgRT[2];									//平均輝度が格納されるレンダリングターゲット。
		int m_currentAvgRt = 0;
		CShaderDx12			m_vs;													//頂点シェーダー。
		CShaderDx12			m_psCalcLuminanceLogAvarageShader;	//輝度の対数平均を求めるピクセルシェーダー。
		CShaderDx12			m_psCalcLuminanceAvarageShader;		//輝度の平均を求めるピクセルシェーダー。
		CShaderDx12			m_psCalcLuminanceExpAvarageShader;	//輝度の指数平均を求めるピクセルシェーダー。
		CShaderDx12			m_psCalcAdaptedLuminanceShader;		//明暗順応のピクセルシェーダー。
		CShaderDx12			m_psCalcAdaptedLuminanceFirstShader;	//明暗順応のピクセルシェーダー。(シーンが切り替わったときに使用される。)
		CShaderDx12			m_psFinal;											//最終合成シェーダー。
		bool						m_isFirstWhenChangeScene = true;			//シーンが切り替わって初回の描画かどうかのフラグ。
		CVector4				m_avSampleOffsets[MAX_SAMPLES];
		CConstantBufferDx12	m_cbCalcLuminanceLog;
		CConstantBufferDx12 m_cbTonemapCommon;
		STonemapParam	m_tonemapParam;
	};
}