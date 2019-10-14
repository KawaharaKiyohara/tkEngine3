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
		auto& ge12 = g_graphicsEngine->As<CGraphicsEngineDx12>();
		auto device = ge12.GetD3DDevice();
		ComPtr< ID3D12DescriptorHeap> heap;
		//�}�e���A�����ƂɃf�B�X�N���v�^�q�[�v���쐬����B
		for (auto& mesh : m_meshs) {
			for (auto& mat : mesh->m_materials) {

				D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
				srvHeapDesc.NumDescriptors = 3;
				srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				auto hr = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&heap));
				TK_ASSERT(SUCCEEDED(hr), "CMeshPartsDx12::CreateDescriptorHeaps�F�f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s���܂����B");

				m_descriptorHeaps.push_back(std::move(heap));
				
			}
		}
	}
	void CMeshPartsDx12::BindSkeleton(CSkeleton& skeleton)
	{
		m_skeleton = &skeleton;
		//�\�����o�b�t�@���쐬����B
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
		//�����_�����O�R���e�L�X�g��Dx12�łɃ_�E���L���X�g
		auto& rc12 = rc.As<CRenderContextDx12>();
		
		//���b�V�����ƂɃh���[
		//�v���~�e�B�u�̃g�|���W�[�̓g���C�A���O�����X�g�̂݁B
		rc12.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�萔�o�b�t�@���X�V����B
		SConstantBuffer cb;
		cb.mWorld = mWorld;
		cb.mView = mView;
		cb.mProj = mProj;

		m_commonConstantBuffer.Update(&cb);

		if (m_boneMatricesStructureBuffer.IsInited()) {
			//�{�[���s����X�V����B
			m_boneMatricesStructureBuffer.Update(m_skeleton->GetBoneMatricesTopAddress());
		}
		int heapNo = 0;
		for (auto& mesh : m_meshs) {
			//���_�o�b�t�@��ݒ�B
			rc12.SetVertexBuffer(mesh->m_vertexBuffer);
			//�}�e���A�����ƂɃh���[�B
			for (int matNo = 0; matNo < mesh->m_materials.size(); matNo++, heapNo++) {
				//���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
				mesh->m_materials[matNo]->BeginRender(rc);

				auto& descriptorHeap = m_descriptorHeaps[heapNo];
				IShaderResourceDx12* srvTbl[] = {
					&mesh->m_materials[matNo]->GetAlbedoMap(),
					&m_boneMatricesStructureBuffer
				};
				auto& albedoMap = mesh->m_materials[matNo]->GetAlbedoMap();
				rc12.SetCBR_SRV_UAV(descriptorHeap.Get(), 1, &m_commonConstantBuffer, 2, srvTbl);

				//�C���f�b�N�X�o�b�t�@��ݒ�B
				auto& ib = mesh->m_indexBufferArray[matNo];
				rc12.SetIndexBuffer(ib);
			
				//�h���[�B
				rc12.DrawIndexed(ib->GetCount());
			}
		}
	}
}
#endif //#if TK_GRAPHICS_API == TK_GRAPHICS_API_DIRECTX_12
