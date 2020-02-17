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
		/// 初期化。
		/// </summary>
		void Init(const SShadowRenderConfig& cfg)
		{
			OnInit(cfg);
		}
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
		/// <summary>
		/// 初期化時に呼ばれる処理。
		/// </summary>
		virtual void OnInit(const SShadowRenderConfig& cfg) = 0;
	private:
		enum { NUN_SHADOW_MAP = 3 };								//シャドウマップの数。
		std::vector<CModel*>	m_shadowCasters;					//シャドウキャスターのリスト。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP] = { g_matIdentity };	//ライトビュープロジェクション行列。
	};

}