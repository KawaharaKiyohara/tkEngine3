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
	/// <summary>
	/// メッシュ
	/// </summary>
	struct SMesh {
		CVertexBufferDx12 m_vertexBuffer;						//頂点バッファ。
		vector< UPCIndexBufferDx12 > m_indexBufferArray;	//インデックスバッファ。
		vector< UPCMaterialDx12 >	m_materials;			//マテリアル。
		vector<int>					skinFlags;				//スキンを持っているかどうかのフラグ。
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
		/// <summary>
		/// スケルトンを関連付ける。
		/// </summary>
		/// <param name="skeleton">スケルトン</param>
		virtual void BindSkeleton(CSkeleton& skeleton) override final;
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
		CConstantBufferDx12 m_commonConstantBuffer;				//メッシュ共通の定数バッファ。
		CStructuredBufferDx12 m_boneMatricesStructureBuffer;	//ボーン行列の構造化バッファ。
		vector< UPSMesh > m_meshs;								//メッシュ。
		vector< CDescriptorHeapDx12 > m_descriptorHeap;			//ディスクリプタヒープ。
		CSkeleton* m_skeleton = nullptr;						//スケルトン。
		bool m_isCreateDescriptorHeap = false;					//ディスクリプタヒープを作成済み？
	};
}

#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
