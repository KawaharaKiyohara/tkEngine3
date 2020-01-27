#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMeshPartsDx12.h"
#include "tkLightManagerDx12.h"

namespace tkEngine {
	void CMeshPartsDx12::InitFromTkmFile(const CTkmFile& tkmFile)
	{
		m_meshs.resize(tkmFile.GetNumMesh());
		int meshNo = 0;
		tkmFile.QueryMeshParts([&](const CTkmFile::SMesh& mesh) {
			//tkmファイルのメッシュ情報からメッシュを作成する。
			CreateMeshFromTkmMesh(mesh, meshNo);
			meshNo++;
			});
		//共通定数バッファの作成。
		m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
	}
	void CMeshPartsDx12::CreateMeshFromTkmMesh(const CTkmFile::SMesh& tkmMesh, int meshNo)
	{
		//頂点バッファを作成。
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(CTkmFile::SVertex);
		auto& mesh = std::make_unique<SMesh>();
		mesh->skinFlags.reserve(tkmMesh.materials.size());
		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

		auto SetSkinFlag = [&](int index) {
			if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
				//スキンがある。
				mesh->skinFlags.push_back(1);
			}
			else {
				//スキンなし。
				mesh->skinFlags.push_back(0);
			}
		};
		//インデックスバッファを作成。
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//インデックスのサイズが2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));
				
				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);
				
				mesh->m_indexBufferArray.push_back(std::move(ib));
			}
		}
		else {
			//インデックスのサイズが4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));

				//スキンがあるかどうかを設定する。
				SetSkinFlag(tkIb.indices[0]);
			
				mesh->m_indexBufferArray.push_back(std::move(ib));
			}
		}
		//マテリアルを作成。
		mesh->m_materials.reserve(tkmMesh.materials.size());
		for (auto& tkmMat : tkmMesh.materials) {
			auto mat = std::make_unique<CMaterialDx12>();
			mat->InitFromTkmMaterila(tkmMat);
			mesh->m_materials.push_back(std::move(mat));
		}

		m_meshs[meshNo] = std::move(mesh);
	}

	void CMeshPartsDx12::BindSkeleton(CSkeleton& skeleton)
	{
		m_skeleton = &skeleton;
		//構造化バッファを作成する。
		m_boneMatricesStructureBuffer.Init(
			sizeof(CMatrix),
			m_skeleton->GetNumBones(),
			m_skeleton->GetBoneMatricesTopAddress()
		);
	}
	void CMeshPartsDx12::Draw(
		IRenderContext& rc, 
		const CMatrix& mWorld, 
		const CMatrix& mView, 
		const CMatrix& mProj)
	{
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		//レンダリングコンテキストをDx12版にダウンキャスト
		auto& rc12 = rc.As<CRenderContextDx12>();
		auto& lightMgr = ge12.GetLightManager()->As<CLightManagerDx12>();

		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		rc12.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファを更新する。
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;

		m_commonConstantBuffer.Update(&cb);

		if (m_boneMatricesStructureBuffer.IsInited()) {
			//ボーン行列を更新する。
			m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
		}

		for (auto& mesh : m_meshs) {
			//頂点バッファを設定。
			rc12.SetVertexBuffer(mesh->m_vertexBuffer);
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//このマテリアルが貼られているメッシュの描画開始。
				mesh->m_materials[matNo]->BeginRender(rc, mesh->skinFlags[matNo]);

				rc12.SetShaderResource(0, mesh->m_materials[matNo]->GetAlbedoMap());
				rc12.SetShaderResource(1, mesh->m_materials[matNo]->GetNormalMap());
				rc12.SetShaderResource(2, mesh->m_materials[matNo]->GetSpecularMap());
				rc12.SetShaderResource(3, m_boneMatricesStructureBuffer);
				rc12.SetShaderResource(4, lightMgr.GetDirectionLightStructuredBuffer());

				rc12.SetConstantBuffer(0, m_commonConstantBuffer);
				rc12.SetConstantBuffer(1, lightMgr.GetLightParamConstantBuffer());
				rc12.SetConstantBuffer(2, mesh->m_materials[matNo]->GetConstantBuffer());

				//インデックスバッファを設定。
				auto& ib = mesh->m_indexBufferArray[matNo];
				rc12.SetIndexBuffer(ib);
			
				//ドロー。
				rc12.DrawIndexed(ib->GetCount());
			}
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
