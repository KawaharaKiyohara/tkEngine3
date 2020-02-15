#pragma once

namespace tkEngine {
	class CModel;
	/// <summary>
	/// 指向性シャドウマップの基底クラス
	/// </summary>
	/// <remarks>
	/// カスケードシャドウマップを採用。
	/// </remarks>
	class IDirectionalShadowMap : Noncopyable {
	public:
		/// <summary>
		/// シャドウマップへのレンダリング。
		/// </summary>
		/// <param name="rc"></param>
		virtual void RenderToShadowMap(IRenderContext& rc) = 0;
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
	private:
		enum { NUN_SHADOW_MAP = 3 };								//シャドウマップの数。
		std::vector<CModel*>	m_shadowCasters;					//シャドウキャスターのリスト。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { g_matIdentity };	//ライトビュープロジェクション行列。
	};

}