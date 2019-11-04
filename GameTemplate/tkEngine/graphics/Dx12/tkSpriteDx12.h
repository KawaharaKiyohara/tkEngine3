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
		/// 初期化時に呼ばれる処理。派生クラスで実装してください。
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		/// <param name="w">幅</param>
		/// <param name="h">高さ</param>
		void OnInit(ITexture* texture, float w, float h) override final;
	
		/// <summary>
		/// 更新時に呼ばれる処理。派生クラスで実装してください。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		/// <param name="pivot">
		/// ピボット
		/// 0.5, 0.5で画像の中心が基点。
		/// 0.0, 0.0で画像の左下。
		/// 1.0, 1.0で画像の右上。
		/// UnityのuGUIに準拠。
		/// </param>
		void OnUpdate(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot) override final;
		/// <summary>
		/// 描画時に呼ばれる処理。派生クラスで実装してください。
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="viewMatrix">ビュー行列</param>
		/// <param name="projMatrix">プロジェクション行列</param>
		void OnDraw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix) override final;

		void InitPipelineState();
		void InitDescriptorHeap();
	private:
		struct SConstantBuffer {
			CMatrix mvp;
			CVector4 mulColor;
		};
		CRootSignatureDx12 m_rootSignature;	//ルートシグネチャ。
		ComPtr< ID3D12PipelineState>	m_pipelineState;	//パイプラインステート。
		CShaderDx12 m_vs;								//頂点シェーダー。
		CShaderDx12 m_ps;								//ピクセルシェーダー。
		SConstantBuffer m_constantBufferCPU;			//CPU側の定数バッファ。
		CConstantBufferDx12 m_constantBufferGPU;		//GPU側の定数バッファ。
		ComPtr< ID3D12DescriptorHeap> m_descriptorHeap;	//ディスクリプタヒープ。
	};
}