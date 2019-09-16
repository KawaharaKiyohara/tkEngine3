#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

#include "tkEngine/graphics/tkTkmFile.h"
namespace tkEngine {
	/// <summary>
	/// マテリアル。
	/// </summary>
	class CMaterialDx12 : Noncopyable{
	public:
		/// <summary>
		/// tkmファイルのマテリアル情報から初期化する。
		/// </summary>
		/// <param name="tkmMat">マテリアル</param>
		void InitFromTkmMaterila(const CTkmFile::SMaterial& tkmMat);
	private:
		CShaderDx12		m_vs;			//頂点シェーダー。
		CShaderDx12		m_ps;			//ピクセルシェーダー。
		CTextureDx12	m_albedoMap;	//アルベドマップ。
		CTextureDx12	m_normalMap;	//法線マップ。
		CTextureDx12	m_specularMap;	//スペキュラマップ。
		ComPtr<ID3D12PipelineState> m_pipelineState;	//パイプラインステート。
	};

	using UPCMaterialDx12 = std::make_unique< CMaterialDx12>;
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12

