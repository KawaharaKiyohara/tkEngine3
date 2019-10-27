#pragma once

#include "tkEngine/graphics/tkIndexbuffer.h"
#include "tkEngine/graphics/tkVertexBuffer.h"

namespace tkEngine {
	/// <summary>
	/// スプライトクラス。
	/// </summary>
	class ISprite : Noncopyable {
	public:
		static const CVector2	DEFAULT_PIVOT;					//!<ピボット。
		virtual ~ISprite();
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		/// <param name="w">幅</param>
		/// <param name="h">高さ</param>
		void Init(ITexture* texture, float w, float h);
		/// <summary>
		/// 更新。
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
		void Update(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot = DEFAULT_PIVOT);
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="viewMatrix">ビュー行列</param>
		/// <param name="projMatrix">プロジェクション行列</param>
		void Draw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);
	private:
		/// <summary>
		/// 初期化時に呼ばれる処理。派生クラスで実装してください。
		/// </summary>
		/// <param name="texture">テクスチャ</param>
		/// <param name="w">幅</param>
		/// <param name="h">高さ</param>
		virtual void OnInit(ITexture* texture, float w, float h) = 0;
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
		virtual void OnUpdate(const CVector3& pos, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot ) = 0;
		/// <summary>
		/// 描画時に呼ばれる処理。派生クラスで実装してください。
		/// </summary>
		/// <param name="renderContext">レンダリングコンテキスト</param>
		/// <param name="viewMatrix">ビュー行列</param>
		/// <param name="projMatrix">プロジェクション行列</param>
		virtual void OnDraw(IRenderContext& renderContext, const CMatrix& viewMatrix, const CMatrix& projMatrix) = 0;
	protected:
		UPIIndexBuffer m_indexBuffer;				//インデックスバッファ。
		UPIVertexBuffer m_vertexBuffer;				//頂点バッファ。
		ITexture* m_texture = nullptr;
		CVector3 m_position = g_vec3Zero;			//座標。
		CVector2 m_size = g_vec2Zero;				//サイズ。
		CQuaternion m_rotation = g_quatIdentity;	//回転。
		CMatrix m_world = g_matIdentity;			//ワールド行列。
	};
}