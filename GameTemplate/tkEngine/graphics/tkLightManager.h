/*!
 * @brief	ライトの管理者。
 */

#pragma once
#include "tkEngine/graphics/tkLight.h"

namespace tkEngine{
	namespace prefab {
		class CDirectionLight;
		class CPointLight;
	//	class CSpotLight;
		class CLightBase;
	}
	/*!
	 * @brief	ライトの管理者。
	 * @details
	 *  インスタンス化されたCLightBaseのインスタンスはすべてCLightManagerに登録されます。
	 *  このクラスのインスタンスはCGraphicsEngineクラスのみが保持しており、その他のクラスでインスタンスを生成することはできません。
	 */
	class ILightManager{
	friend class CGraphicsEngine;
	
	public:
		void Init();
		/*!
		*@brief　ライトを追加。
		*/
		void AddLight(prefab::CLightBase* light);
		/*!
		*@brief　ライトを削除。
		*/
		void RemoveLight(prefab::CLightBase* light);
		/*!
		*@brief　更新。
		*/
		void Update();

		/// <summary>
		/// 描画シーケンスから呼ばれる処理。
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		void Render(IRenderContext& renderContext);
#if 0//todo
		/*!
		*@brief　１フレームの描画終了時に呼ばれる処理。
		*/
		void EndRender(CRenderContext& renderContext);
#endif
		/*!
		*@brief　ポイントライトの数を取得。
		*/
		int GetNumPointLight() const
		{
			return static_cast<int>(m_pointLights.size());
		}

		/*!
		 *@brief	アンビエントライトを取得。
		 */
		const CVector3& GetAmbientLight() const
		{
			return m_lightParam.ambientLight;
		}
		/*!
		 *@brief	アンビエントライトを設定。
		 */
		void SetAmbientLight(const CVector3& ambient)
		{
			m_lightParam.ambientLight = ambient;
		}
		/// <summary>
		/// GPUにライトの情報を転送します。
		/// </summary>
		virtual void SendLightDataToGPU(IRenderContext& rc) = 0;
		/// <summary>
		/// ILightManagerの派生クラスに型変換。
		/// </summary>
		template<class T>
		T& As()
		{
			return dynamic_cast<T&>(*this);
		}
	private:
		
		/// <summary>
		/// 初期化時に呼ばれるコード。
		/// </summary>
		virtual void OnInit() = 0;
		/// <summary>
		/// 描画時に呼ばれるコード。
		/// </summary>
		virtual void OnRender(IRenderContext& rc) = 0;
	protected:
		static const int MAX_DIRECTION_LIGHT = 8;						//!<ディレクションライトの最大数。
		static const int MAX_POINT_LIGHT = 1024;						//!<ポイントライトの最大数。
		/*!
		 *@brief	GPUで使用するライト用のパラメータ。
		 *@details
		 * メンバ変数を追加したり、並びを変えたりしたら
		 * Assets/shader/modelCB.hのlightCbも変える必要あり。
		 */
		struct SLightParam {
			CVector3 eyePos;			//視線の位置。
			int numDirectionLight;		//ディレクションライトの数。
			CVector3 ambientLight;		//アンビエントライト。
			int numPointLight;			//ポイントライトの数。
			CVector4 screenParam;		//スクリーンパラメータ。
		};
		SLightParam							m_lightParam;
		SDirectionLight						m_rawDirectionLights[MAX_DIRECTION_LIGHT];
		SPointLight							m_rawPointLights[MAX_POINT_LIGHT];
		std::list<prefab::CDirectionLight*>	m_directionLights;		//!<平行光源のリスト。
		std::list<prefab::CPointLight*>		m_pointLights;			//!<ポイントライトのリスト。
	};
}