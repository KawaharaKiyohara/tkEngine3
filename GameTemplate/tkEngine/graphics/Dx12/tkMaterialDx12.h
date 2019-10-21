#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkTkmFile.h"
namespace tkEngine {
	/// <summary>
	/// マテリアル。
	/// </summary>
	class CMaterialDx12 : Noncopyable {
	public:
		/// <summary>
		/// tkmファイルのマテリアル情報から初期化する。
		/// </summary>
		/// <param name="tkmMat">tkmマテリアル</param>
		void InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat);
		/// <summary>
		/// レンダリングを開始するときに呼び出す関数。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="hasSkin">スキンがあるかどうかのフラグ</param>
		void BeginRender(IRenderContext& rc, int hasSkin);

		/// <summary>
		/// アルベドマップを取得。
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetAlbedoMap()
		{
			return m_albedoMap;
		}
		/// <summary>
		/// 法線マップを取得。
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetNormalMap()
		{
			return m_normalMap;
		}
		/// <summary>
		/// スペキュラマップを取得。
		/// </summary>
		/// <returns></returns>
		CTextureDx12& GetSpecularMap()
		{
			return m_specularMap;
		}
		/// <summary>
		/// 定数バッファを取得。
		/// </summary>
		/// <returns></returns>
		CConstantBufferDx12& GetConstantBuffer()
		{
			return m_constantBuffer;
		}
	private:
		/// <summary>
		/// パイプラインステートを初期化。
		/// </summary>
		/// <param name="tkmMat">tkmマテリアル</param>
		void InitPipelineState(const CTkmFile::SMaterial& tkmMat);
		/// <summary>
		/// ルートシグネチャを初期化。
		/// </summary>
		/// <param name="tkmMat">tkmMat</param>
		void InitRootSignature(const CTkmFile::SMaterial& tkmMat);
		/// <summary>
		/// テクスチャを初期化。
		/// </summary>
		/// <param name="tkmMat"></param>
		void InitTexture(const CTkmFile::SMaterial& tkmMat);
	private:
		/// <summary>
		/// マテリアルパラメータ。
		/// </summary>
		struct SMaterialParam {
			int hasNormalMap;	//法線マップを保持しているかどうかのフラグ。
			int hasSpecMap;		//スペキュラマップを保持しているかどうかのフラグ。
		};
		CShaderDx12		m_vsNonSkin;	//スキンなしモデルの頂点シェーダー。
		CShaderDx12		m_vs;			//頂点シェーダー。
		CShaderDx12		m_ps;			//ピクセルシェーダー。
		CTextureDx12	m_albedoMap;	//アルベドマップ。
		CTextureDx12	m_normalMap;	//法線マップ。
		CTextureDx12	m_specularMap;	//スペキュラマップ。
		ComPtr<ID3D12RootSignature> m_rootSignature;		//ルートシグネチャ。
		ComPtr<ID3D12PipelineState> m_pipelineState;			//パイプラインステート。
		ComPtr<ID3D12PipelineState> m_pipelineStateNonSkin;		//スキンなしモデルを描画する時のパイプラインステート。
		CConstantBufferDx12 m_constantBuffer;	//定数バッファ。
	};

	using UPCMaterialDx12 = unique_ptr< CMaterialDx12>;
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

