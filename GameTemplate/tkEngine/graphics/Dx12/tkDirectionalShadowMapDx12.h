#pragma once


#include "tkEngine/graphics/tkDirectionalShadowMap.h"

namespace tkEngine {
	class CModel;
	class IRenderContext;
	/// <summary>
	/// 指向性シャドウマップの基底クラス
	/// </summary>
	/// <remarks>
	/// カスケードシャドウマップを採用。
	/// </remarks>
	class CDirectionalShadowMapDx12 : public IDirectionalShadowMap {
	public:
		
	private:
		/// <summary>
		/// 初期化処理。
		/// </summary>
		void OnInit(const SShadowRenderConfig& cfg) override final;
		/// <summary>
		/// シャドウマップへのレンダリング処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderToShadowMap(IRenderContext& rc) override final;
	private:
		enum { NUN_SHADOW_MAP = 3 };								//シャドウマップの数。
		std::vector<CModel*>	m_shadowCasters;					//シャドウキャスターのリスト。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { g_matIdentity };	//ライトビュープロジェクション行列。
		CRenderTargetDx12 m_shadowMaps[NUM_SHADOW_MAP];				//シャドウマップ。
	};
}