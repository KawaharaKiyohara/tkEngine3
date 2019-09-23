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
		/// <param name="mWorld">ワールド行列</param>
		/// <param name="mView">ビュー行列</param>
		/// <param name="mProj">プロジェクション行列</param>
		virtual void Draw(IRenderContext& rc, const CMatrix& mWorld, const CMatrix& mView, const CMatrix& mProj) override final;
	private:
		/// <summary>
		/// tkmメッシュからメッシュを作成。
		/// </summary>
		/// <param name="mesh">メッシュ</param>
		/// <param name="meshNo">メッシュ番号</param>
		void CreateMeshFromTkmMesh(const CTkmFile::SMesh& mesh, int meshNo);
		/// <summary>
		/// 共通定数バッファの作成。
		/// </summary>
		void CreateCommonConstantBuffer();
		/// <summary>
		/// ディスクリプタヒープを作成。
		/// </summary>
		void CreateDescriptorHeaps();
	private:
		/// <summary>
		/// 定数バッファ。
		/// </summary>
		/// <remarks>
		/// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
		/// </remarks>
		struct SConstantBuffer {
			CMatrix mWorld;		//ワールド行列。
			CMatrix mView;		//ビュー行列。
			CMatrix mProj;		//プロジェクション行列。
		};
		CConstantBufferDx12 m_commonConstantBuffer;		//メッシュ共通の定数バッファ。
		std::vector< UPSMesh > m_meshs;					//メッシュ。
		std::vector<ComPtr< ID3D12DescriptorHeap>>	m_descriptorHeaps;
		int m_cbrSrvDescriptorSize = 0;	
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
