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
		//共通定数バッファの作成。
		m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
		//ディスクリプタヒープを作成。
		CreateDescriptorHeaps();
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
	void CMeshPartsDx12::CreateDescriptorHeaps()
	{
		auto ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12->GetD3DDevice();
		m_cbrSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		ComPtr< ID3D12DescriptorHeap> heap;
		//マテリアルごとにディスクリプタヒープを作成する。
		for (auto& mesh : m_meshs) {
			for (auto& mat : mesh->m_materials) {

				D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
				srvHeapDesc.NumDescriptors = 2;
				srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				auto hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&heap));
				TK_ASSERT(SUCCEEDED(hr), "CMeshPartsDx12::CreateDescriptorHeaps：ディスクリプタヒープの作成に失敗しました。");
				//ディスクリプタヒープに定数バッファとテクスチャを登録してく。
				auto handle = heap->GetCPUDescriptorHandleForHeapStart();
				m_commonConstantBuffer.RegistConstantBufferView(handle);
				handle.ptr += m_cbrSrvDescriptorSize;
				auto& albedoMap = mat->GetAlbedoMap();
				albedoMap.RegistShaderResourceView(handle);

				m_descriptorHeaps.push_back(std::move(heap));
				
			}
		}
	}
	void CMeshPartsDx12::Draw(
		IRenderContext& rc, 
		const CMatrix& mWorld, 
		const CMatrix& mView, 
		const CMatrix& mProj)
	{
		//レンダリングコンテキストをDx12版にダウンキャスト
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		
		//メッシュごとにドロー
		//プリミティブのトポロジーはトライアングルリストのみ。
		rcDx12.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファを更新する。
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;

		m_commonConstantBuffer.Update(&cb);

		int heapNo = 0;
		for (auto& mesh : m_meshs) {
			//頂点バッファを設定。
			rcDx12.IASetVertexBuffer(mesh->m_vertexBuffer);
			//マテリアルごとにドロー。
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//このマテリアルが貼られているメッシュの描画開始。
				mesh->m_materials[matNo]->BeginRender(rc);

				auto& descriptorHeap = m_descriptorHeaps[heapNo];
				heapNo++;
				rcDx12.SetDescriptorHeap(descriptorHeap);
				auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
				//ディスクリプタヒープをルートシグネチャに登録していく。
				rcDx12.SetGraphicsRootDescriptorTable(
					0,
					gpuHandle
				);
				auto& albeoMap = mesh->m_materials[matNo]->GetAlbedoMap();
				if (albeoMap.IsValid()) {
					gpuHandle.ptr += m_cbrSrvDescriptorSize;
					rcDx12.SetGraphicsRootDescriptorTable(
						1,
						gpuHandle
					);
				}

				//インデックスバッファを設定。
				auto& ib = mesh->m_indexBufferArray[matNo];
				rcDx12.IASetIndexBuffer(ib);
				
				//ドロー。
				rcDx12.DrawIndexed(ib->GetCount());
		
				mesh->m_materials[matNo]->EndRender(rc);
			}
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
