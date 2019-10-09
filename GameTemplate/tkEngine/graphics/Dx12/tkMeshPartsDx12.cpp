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
			//tkm�t�@�C���̃��b�V����񂩂烁�b�V�����쐬����B
			CreateMeshFromTkmMesh(mesh, meshNo);
			meshNo++;
			});
		//���ʒ萔�o�b�t�@�̍쐬�B
		m_commonConstantBuffer.Init(sizeof(SConstantBuffer), nullptr);
		//�f�B�X�N���v�^�q�[�v���쐬�B
		CreateDescriptorHeaps();
	}
	void CMeshPartsDx12::CreateMeshFromTkmMesh(const CTkmFile::SMesh& tkmMesh, int meshNo)
	{
		//���_�o�b�t�@���쐬�B
		int numVertex = (int)tkmMesh.vertexBuffer.size();
		int vertexStride = sizeof(CTkmFile::SVertex);
		auto& mesh = std::make_unique<SMesh>();

		mesh->m_vertexBuffer.Init(vertexStride * numVertex, vertexStride);
		mesh->m_vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);
		//�C���f�b�N�X�o�b�t�@���쐬�B
		if (!tkmMesh.indexBuffer16Array.empty()) {
			//�C���f�b�N�X�̃T�C�Y��2byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer16Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 2, 2);
				ib->Copy((void*)&tkIb.indices.at(0));
				mesh->m_indexBufferArray.push_back(std::move(ib));
			}
		}
		else {
			//�C���f�b�N�X�̃T�C�Y��4byte
			mesh->m_indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
			for (auto& tkIb : tkmMesh.indexBuffer32Array) {
				auto ib = std::make_unique< CIndexBufferDx12>();
				ib->Init(tkIb.indices.size() * 4, 4);
				ib->Copy((void*)&tkIb.indices.at(0));
				mesh->m_indexBufferArray.push_back(std::move(ib));
			}
		}
		//�}�e���A�����쐬�B
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
		//�}�e���A�����ƂɃf�B�X�N���v�^�q�[�v���쐬����B
		for (auto& mesh : m_meshs) {
			for (auto& mat : mesh->m_materials) {

				D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
				srvHeapDesc.NumDescriptors = 2;
				srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				auto hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&heap));
				TK_ASSERT(SUCCEEDED(hr), "CMeshPartsDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");
				//�f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�ƃe�N�X�`����o�^���Ă��B
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
		//�����_�����O�R���e�L�X�g��Dx12�łɃ_�E���L���X�g
		auto& rcDx12 = rc.As<CRenderContextDx12>();
		
		//���b�V�����ƂɃh���[
		//�v���~�e�B�u�̃g�|���W�[�̓g���C�A���O�����X�g�̂݁B
		rcDx12.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�萔�o�b�t�@���X�V����B
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;

		m_commonConstantBuffer.Update(&cb);

		int heapNo = 0;
		for (auto& mesh : m_meshs) {
			//���_�o�b�t�@��ݒ�B
			rcDx12.IASetVertexBuffer(mesh->m_vertexBuffer);
			//�}�e���A�����ƂɃh���[�B
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++) {
				//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
				mesh->m_materials[matNo]->BeginRender(rc);

				auto& descriptorHeap = m_descriptorHeaps[heapNo];
				heapNo++;
				rcDx12.SetDescriptorHeap(descriptorHeap);
				auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
				//�f�B�X�N���v�^�q�[�v�����[�g�V�O�l�`���ɓo�^���Ă����B
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

				//�C���f�b�N�X�o�b�t�@��ݒ�B
				auto& ib = mesh->m_indexBufferArray[matNo];
				rcDx12.IASetIndexBuffer(ib);
				
				//�h���[�B
				rcDx12.DrawIndexed(ib->GetCount());
		
				mesh->m_materials[matNo]->EndRender(rc);
			}
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
