/*!
 * @brief	ライトの管理者。
 */

#pragma once
#include "tkEngine/graphics/tkLightManager.h"

namespace tkEngine{
	
	/*!
	 * @brief	DirectX12版のライトの管理者。
	 */
	/// <summary>
	/// DirectX12版のライトの管理者。
	/// </summary>

	class CLightManagerDx12 : public ILightManager{	
	public:
		/// <summary>
		/// 共通ライトパラメータの定数バッファを取得。
		/// </summary>
		/// <returns></returns>
		CConstantBufferDx12& GetLightParamConstantBuffer()
		{
			return m_lightParamCB;
		}
		/// <summary>
		/// ディレクションライトのストラクチャーバッファを取得。
		/// </summary>
		/// <returns></returns>
		CStructuredBufferDx12& GetDirectionLightStructuredBuffer()
		{
			return m_directionLightSB;
		}
	private:
		/// <summary>
		/// GPUにライトの情報を転送します。
		/// </summary>
		void SendLightDataToGPU(IRenderContext& rc) override final;
		/// <summary>
		/// 初期化時に呼ばれる処理。
		/// </summary>
		void OnInit() override final;
		/// <summary>
		/// 描画時に呼ばれるコード。
		/// </summary>
		void OnRender(IRenderContext& rc) override final;
	private:
		/// <summary>
		/// モデルシェーダーで使用するライト用のパラメータ。
		/// </summary>
		/// <remarks>
		/// メンバ変数を追加したり、並びを変えたりしたら
		/// Assets / shader / modelCB.hのlightCbも変える必要あり。
		/// </remarks>
		struct SLightParam {
			CVector3 eyePos;			//視線の位置。
			int numDirectionLight;		//ディレクションライトの数。
			CVector3 ambientLight;		//アンビエントライト。
			int numPointLight;			//ポイントライトの数。
			CVector4 screenParam;		//スクリーンパラメータ。
		};
		CConstantBufferDx12 m_lightParamCB;			//定数バッファ。
		CStructuredBufferDx12 m_directionLightSB;	//ディレクションライトのストラクチャバッファ。
	};
}