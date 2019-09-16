#pragma once

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12


#include "tkEngine/graphics/tkMeshParts.h"
#include "tkEngine/graphics/Dx12/tkVertexBufferDx12.h"
#include "tkEngine/graphics/Dx12/tkIndexBufferDx12.h"
#include <vector>
#include "tkEngine/graphics/tkTkmFile.h"
#include "tkEngine/graphics/Dx12/tkShaderDx12.h"
#include "tkEngine/graphics/Dx12/tkTextureDx12.h"
#include "tkEngine/graphics/Dx12/tkMaterialDx12.h"

namespace tkEngine {
	using UPCIndexBufferDx12 = std::unique_ptr<CIndexBufferDx12>;
	/// <summary>
	/// メッシュ
	/// </summary>
	struct SMesh {
		CVertexBufferDx12 m_vertexBuffer;						//頂点バッファ。
		std::vector< UPCIndexBufferDx12 > m_indexBufferArray;	//インデックスバッファ。
		std::vector< UPCMaterialDx12 >	m_materials;				//マテリアル。
	};
	using UPSMesh = std::unique_ptr<SMesh>;
	/// <summary>
	/// メッシュパーツ。
	/// </summary>
	class CMeshPartsDx12 : public IMeshParts {
	public:
		/// <summary>
		/// tkmファイルから初期化。
		/// </summary>
		/// <param name="tkmFile"></param>
		void InitFromTkmFile(const CTkmFile& tkmFile) override final;
		
		/// <summary>
		/// 描画。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// tkmメッシュからメッシュを作成。
		/// </summary>
		/// <param name="mesh">メッシュ</param>
		/// <param name="meshNo">メッシュ番号</param>
		void CreateMeshFromTkmMesh(const CTkmFile::SMesh& mesh, int meshNo);
	private:
		std::vector< UPSMesh > m_meshs;		//メッシュ。
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
