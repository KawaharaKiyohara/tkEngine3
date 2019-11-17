#pragma once

#include "tkEngine/graphics/tkPrimitive.h"
#include "tkRenderContextDx12.h"

namespace tkEngine {
	/// <summary>
	/// ブルーム。
	/// </summary>
	class CBloomDx12 : Noncopyable {
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
	private:
		/// <summary>
		/// 重みを更新。
		/// </summary>
		/// <param name="dispersion"></param>
		void UpdateWeight(float dispersion);
		/// <summary>
		/// レンダリングターゲットを初期化。
		/// </summary>
		void InitRenderTargets();
		/// <summary>
		/// 四角形プリミティブを初期化。
		/// </summary>
		void InitQuadPrimitive();
		/// <summary>
		/// シェーダーを初期化。
		/// </summary>
		void InitShaders();
		/// <summary>
		/// パイプラインステートを初期化。
		/// </summary>
		void InitPipelineState();
		/// <summary>
		/// 輝度を抽出
		/// </summary>
		/// <param name="rc"></param>
		void SamplingLuminance(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12);
		/// <summary>
		/// 輝度テクスチャにブラーをかける。
		/// </summary>
		/// <param name="ge12"></param>
		/// <param name="rc12"></param>
		void BlurLuminanceTexture(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12);
		/// <summary>
		/// ボケ画像を合成する。
		/// </summary>
		/// <param name="ge12"></param>
		/// <param name="rc12"></param>
		void CombineBokeImage(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12);
		/// <summary>
		/// メインレンダリングターゲットへの合成。
		/// </summary>
		/// <param name="ge12"></param>
		/// <param name="rc12"></param>
		void CombineMainRenderTarget(CGraphicsEngineDx12& ge12, CRenderContextDx12& rc12);
		/// <summary>
		/// ディスクリプタヒープの初期化。
		/// </summary>
		void InitDescriptorHeap();
	private:
		static const int NUM_WEIGHTS = 8;
		static const int NUM_DOWN_SAMPLING_RT = 10;
		/// <summary>
		/// ブラー用のパラメータ。
		/// </summary>
		struct SBlurParam {
			CVector4 offset;
			float weights[NUM_WEIGHTS];
		};
		CRootSignatureDx12 m_rootSignature;	//ブルーム描画用のルートシグネチャ。
		ComPtr< ID3D12DescriptorHeap> m_luminanceDescriptorHeap;	//輝度抽出時に使うディスクリプタヒープ。
		ComPtr< ID3D12DescriptorHeap> m_downSamplingDescriptorHeap[NUM_DOWN_SAMPLING_RT];	//ダウンサンプリング時に使うディスクリプタヒープ。
		ComPtr< ID3D12DescriptorHeap> m_combineBokeImageDescriptorHeap;	//ボケ画像合成時に使うディスクリプタヒープ。
		ComPtr< ID3D12DescriptorHeap> m_combineMainRenderTargetDescriptorHeap;	//メインレンダリングターゲットへの合成ステップで使うディスクリプタヒープ。

		ComPtr<ID3D12PipelineState> m_samplingLuminancePipelineState;	//輝度抽出ステップのパイプラインステート。
		ComPtr<ID3D12PipelineState> m_xblurLuminancePipelineState;		//x方向に輝度をぼかすステップのパイプラインステート。
		ComPtr<ID3D12PipelineState> m_yblurLuminancePipelineState;		//y方向に輝度をぼかすステップのパイプラインステート。
		ComPtr<ID3D12PipelineState> m_combineBokeImagePipelineState;	//ボケ画像を合成するステップのパイプラインステート。
		ComPtr<ID3D12PipelineState> m_combineMainRenderTargetPipelineState;			//メインレンダリングターゲットへの合成ステップのパイプラインステート。
		
		CPrimitive m_quadPrimitive;			//四角形プリミティブ。
		CRenderTargetDx12 m_luminanceRT;	//輝度を抽出するためのレンダリングターゲット。
		CRenderTargetDx12 m_combineRT;		//ぼかし合成用のレンダリングターゲット。
		CRenderTargetDx12 m_downSamplingRT[NUM_DOWN_SAMPLING_RT];	//ダウンサンプリング用のレンダリングターゲット。
		SBlurParam m_blurParam;			//ブラー用のパラメータ。
		CConstantBufferDx12 m_cbBlur;
		CShaderDx12 m_vs;				//頂点シェーダー。
		CShaderDx12 m_psLuminance;		//輝度抽出パスのピクセルシェーダー。
		CShaderDx12 m_vsXBlur;			//X方向ブラーパスの頂点シェーダー。
		CShaderDx12 m_vsYBlur;			//Y方向ブラーパスの頂点シェーダー。
		CShaderDx12 m_psBlur;			//X方向ブラー/Y方向ブラーパスのピクセルシェーダー。
		CShaderDx12 m_psCombine;		//合成パスのピクセルシェーダー。
		CShaderDx12 m_copyVS;			//コピー用の頂点シェーダー。	
		CShaderDx12 m_copyPS;			//コピー用のピクセルシェーダー。
		CConstantBufferDx12 m_blurParamCB[NUM_DOWN_SAMPLING_RT];	//ブラー用の定数バッファ。

	};
}