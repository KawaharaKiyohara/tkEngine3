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
			m_isEnable = cfg.isEnable;
			OnInit(cfg);
		}
		/// <summary>
		/// シャドウマップへのレンダリング。
		/// </summary>
		/// <param name="rc"></param>
		void RenderToShadowMap(IRenderContext& rc)
		{
			if (m_isEnable == false) {
				return;
			}
			//レンダリングステップをシャドウマップ作成に変更する。
			rc.SetRenderStep(enRenderStep_CreateDirectionalShadowMap);
			OnRenderToShadowMap(rc);
		}
		/// <summary>
		/// シャドウマップへのレンダリングの完了待ち。
		/// </summary>
		/// <remarks>
		/// レンダリングの完了待ちが必要なプラットフォームで実装してください。
		/// </remarks>
		/// <param name="rc"></param>
		virtual void WaitEndRenderToShadowMap(IRenderContext& rc) = 0;
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// シャドウマップを有効にする。
		/// </summary>
		void SetEnable()
		{
			m_isEnable = true;
		}
		/// <summary>
		/// シャドウマップを無効にする。
		/// </summary>
		void SetDisable()
		{
			m_isEnable = false;
		}
	private:
		/// <summary>
		/// ライトの座標を計算する。
		/// </summary>
		/// <remarks>
		/// 分割された視推台を写すライトの座標を計算します。
		/// ライトの座標の計算はライトの高さ、ライトの方向、そして、
		/// 視推台の中心座標(近平面の中心座標と遠平面の中心座標を結んだ線分の中点座標)
		/// から計算されます。
		/// これらのパラメータをもとにライトの座標は下記の計算で求められます。
		/// 
		/// ①　未知数αについての一次方程式を解きます。
		/// ライトの高さ = 視推台の中心座標.y + ライトの方向.y * α
		/// α = ( ライトの高さ - 視推台の中心座標.y ) / ライトの方向.y
		/// ②　αを使って、ライトの座標を求める。
		/// ライトの座標 = 視推台の中心座標 + ライトの方向 * α
		/// </remarks>
		/// <param name="lightHeight">
		/// ライトの高さ。
		/// </param>
		/// <param name="viewFrustomCenterPosition">
		/// 分割された視推台の中心座標。
		/// </param>
		/// <returns>計算されたライトの座標</returns>
		CVector3 CalcLightPosition(float lightHeight, CVector3 viewFrustomCenterPosition);
		/// <summary>
		/// 初期化時に呼ばれる処理。
		/// </summary>
		virtual void OnInit(const SShadowRenderConfig& cfg) = 0;
		/// <summary>
		/// シャドウマップへのレンダリング時に呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRenderToShadowMap(IRenderContext& rc) = 0;
	protected:
		/// <summary>
		/// この中身を変更したら、modelCB.hのShadowCbも変更するように。
		/// </summary>
		struct SShadowCb {
			CMatrix mLVP[NUM_SHADOW_MAP];
			CVector4 texOffset[NUM_SHADOW_MAP];
			float depthOffset[NUM_SHADOW_MAP];
			float pading;
			float shadowAreaDepthInViewSpace[NUM_SHADOW_MAP];	//カメラ空間での影を落とすエリアの深度テーブル。
		};
		enum { NUN_SHADOW_MAP = 3 };					//シャドウマップの数。
		std::vector<CModel*>	m_shadowCasters;		//シャドウキャスターのリスト。
		CMatrix	m_LVPMatrix[NUM_SHADOW_MAP];			//ライトビュープロジェクション行列。
		SShadowCb m_shadowCbEntity;
	private:
		
		CVector3 m_lightDirection;						//ライトの方向。
		float m_shadowAreas[NUM_SHADOW_MAP] = { 0.0f };	//影が落ちる範囲。
		float m_lightHeight = 100.0f;					//ライトの高さ。
		bool m_isEnable = false;						//シャドウマップが有効かどうか。
	};

}