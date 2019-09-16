#include "tkEngine/tkEnginePreCompile.h"

#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
#include "dx12Common.h"
#include "tkMeshPartsDx12.h"

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
	}
	void CMeshPartsDx12::CreateMeshFromTkmMesh(const CTkmFile::SMesh& tkmMesh, int meshNo)
	{
		//頂点バッファを作成。
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(CTkmFile::SVertex);
		auto& mesh = std::make_unique<SMesh>();
		
		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);
		//インデックスバッファを作成。
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//インデックスのサイズが2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));
				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		else {
			//インデックスのサイズが4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));
				mesh->m_indexBufferArray.push_back(ib);
			}
		}
		//マテリアルを作成。
		mesh->m_materials.reserve(tkmMesh.materials.size());
		for (auto& tkmMat : tkmMesh.materials) {
			auto mat = std::make_unique<CMaterialDx12>();
			mat->InitFromTkmMaterila(tkmMat);
			mesh->m_materials.push_back(mat);
		}

		m_meshs[meshNo] = std::move(mesh);
	}
	void CMeshPartsDx12::Draw(IRenderContext& rc, const CMatrix& mView, const CMatrix& mProj)
	{
		//レンダリングコンテキストをDx12版にダウンキャスト
		auto rcDx12 = rc.As<CRenderContextDx12>();
		//コマンドリストを取得。
		auto commandList = rcDx12->GetCommandList();
		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (auto& mesh : m_meshs) {
			//頂点バッファを設定。
			commandList->IASetVertexBuffers(0, 1, &mesh->m_vertexBuffer.GetView());
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//インデックスバッファを設定。
				auto& ib = mesh->m_indexBufferArray[matNo];
				commandList->IASetIndexBuffer(&ib->GetView());
				//ドローコール。
				commandList->DrawIndexedInstanced(ib->GetCount(), 1, 0, 0, 0);
			}
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
