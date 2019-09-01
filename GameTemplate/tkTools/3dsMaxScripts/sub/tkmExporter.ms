/*!
 * @brief	tkmエクスポーター。
 */
 

--tkmファイルのバージョン
TKM_VERSION = 100

struct Vertex(
	pos = Point3,		--頂点座標
	normal = Point3,	--法線
	uv = Point2,		--UV座標
	weights = #(),		--スキンウェイト
	indices = #()		--ボーンインデックス
)
--マテリアル。
struct tkMaterial(
	id,					--マテリアルID
	albedoMap = string,	--アルベドマップ
	normalMap = string, --法線マップ
	specMap = string	--スペキュラマップ
)
struct IndexBuffer(
	indices = #()
)
--メッシュパーツ。
struct MeshParts(
	materials = #(),		--マテリアル。
	vertexBuffer = #(),		--頂点バッファ。
	indexBufferArray = #()	--インデックスバッファの配列(マテリアルの数分だけあります。)
)

--モデルデータ。
struct ModelData(
	meshParts = #()		--メッシュパーツ。
)

struct TkmExporter(
private
	md = ModelData,	--メッシュデータ。
	ms = Meshparts,	--メッシュパーツ。

	--頂点バッファの構築。
	fn BuildMeshPatrs_VertexBuffer c_node meshObj =(
		numVert = getNumVerts meshObj
		for vertNo = 1 to numVert do(
			--頂点座標
			pos = getVert meshObj vertNo
			--法線
			normal = getNormal meshObj vertNo
			--正規化されてないね。
			normal = normalize normal
			--UV座標
			uvw = gettvert meshObj vertNo
			uv = Point2 uvw.x uvw.y
			--スキンウェイト
			skinWeight = #(0.0, 0.0, 0.0, 0.0)
			indices = #(-1, -1, -1, -1)
			skinMod = c_node.modifiers[skin]
			if skinMod != undefined then(
				--スキンあり。
				max modify mode
				modPanel.setCurrentObject skinMod
				numWeight = skinOps.GetVertexWeightCount skinMod vertNo
				if numWeight > 4 then (
					--スキンウェイトの数が4を超えています。
					numWeight = 4
				)
				for skinIndex = 1 to numWeight do (
					skinWeight[skinIndex] = skinOps.GetVertexWeight skinMod vertNo skinIndex
					sysytemBoneIndex = skinOps.GetVertexWeightBoneID skinMod vertNo skinIndex
					--第三引数は0でいいのかどうかか不明。
					--なにか問題が起きたら1にしてみよう。
					boneName = skinOps.GetBoneName skinMod sysytemBoneIndex 0
					boneNo = GetBoneIndex boneName
					indices[skinIndex] = boneNo
				)
				--スキンウェイトが4以上なら正規化する？
			)	
			--頂点作成
			v = vertex pos:pos normal:normal uv:uv weights:skinWeight indices:indices
			
			append ms.vertexBuffer v
		)
	),
	--マテリアル情報を構築する。
	fn BuildMeshParts_Material c_node meshObj numFace  =
	(
		numSubMaterial = getNumSubMtls c_node.material
		matIds = #()
		for faceNo = 1 to numFace do (
			matId  = getFaceMatID meshObj faceNo
			if numSubMaterial > 0 then(
				regist = 0
				--登録済みのマテリアルかどうか調べる。
				for tkMat in ms.materials do (
					if matId == tkMat.id then(
						--登録済み
						regist = 1
					)
				)
				if regist == 0 then(
					--未登録のマテリアルなので、追加する。
					mat = getSubMtl c_node.material matId
					diffMap = mat.diffuseMap
					albedoFileName = diffMap.fileName
					newMat = tkMaterial albedoMap:albedoFileName id:matId
					append ms.materials newMat
				)
			)
		)
	),
	--インデックスバッファを構築する。
	--インデックスバッファの数はマテリアルの数分だけ構築されます。
	fn BuildMeshParts_IndexBuffer c_node meshObj numFace = 
	(
		--マテリアルの数分だけインデックスバッファを作成する。
		for tkMat in ms.materials do (
			indices = #()
			for faceNo = 1 to numFace do (
				matId  = getFaceMatID meshObj faceNo
				if tkMat.id == matId then(
					--現在調査中のマテリアルIDと同じ？
					--三角形を構成するインデックスを取得する。
					vertNos = getFace meshObj faceNo
					append indices vertNos[1]
					append indices vertNos[2]
					append indices vertNos[3]
				)
			)	
			--インデックスバッファを追加＾－＾
			append ms.indexBufferArray indices
		)
	),
	--メッシュパーツをビルドする。
	fn BuildMeshParts c_node = 
	(
		--メッシュ―パーツを作成。
		ms = Meshparts materials:#() indexBufferArray:#() vertexBuffer:#()	
		--集約しないとサブマテリアルＩＤが取れないぞいぞい。
		meshObj = copy c_node 
		meshObj = convertToMesh meshObj
		numFace = getNumFaces c_node
		
		--マテリアル情報から構築する。
		BuildMeshParts_Material c_node meshObj numFace

		--インデックスバッファを構築する。
		BuildMeshParts_IndexBuffer c_node meshObj numFace

		--頂点バッファを構築する。
		BuildMeshPatrs_VertexBuffer c_node meshObj

		--メッシュパーツを追加。
		append md.meshParts ms

		delete meshObj
	),
	fn InternalSave filePath = (
		--保存だよ
		file = fopen filePath "wb"
		--try(
			--ヘッダーを出力
			WriteShort file TKM_VERSION 
			--メッシュパーツの数を出力
			WriteShort file md.meshParts.count
			--メッシュパーツを出力していく。
			for meshNo = 1 to md.meshParts.count do(
				--マテリアルの数を出力する。
				numMat = md.meshParts[meshNo].materials.count
				WriteLong file numMat
				--マテリアルの情報を出力する。
				--todo これはちょっと後。

				--頂点数を出力。
				numVertex = md.meshParts[meshNo].vertexBuffer.count
				WriteLong file numVertex

				--インデックスのサイズを出力
				indexSize = 0
				if numVertex < 65536  then (
					--2byte
					indexSize = 2
				)else(
					--4byte
					indexSize = 4
				)
				WriteByte file indexSize
				--パディング
				WriteByte file 0
				WriteByte file 0
				WriteByte file 0

				--頂点バッファを出力
				for v in md.meshParts[meshNo].vertexBuffer do (
					--頂点座標を出力。
					WriteFloat file v.pos.x
					WriteFloat file v.pos.y
					WriteFloat file v.pos.z
					--頂点法線を出力。
					WriteFloat file v.normal.x
					WriteFloat file v.normal.y
					WriteFloat file v.normal.Z
					--UV座標を出力
					WriteFloat file v.uv.x
					WriteFloat file v.uv.y
					--スキンウェイトを出力
					WriteFloat file v.weights[1]
					WriteFloat file v.weights[2]
					WriteFloat file v.weights[3]
					WriteFloat file v.weights[4]
					--ボーンインデックスを出力。
					WriteShort file v.indices[1]
					WriteShort file v.indices[2]
					WriteShort file v.indices[3]
					WriteShort file v.indices[4]
				)

				--インデックスバッファを出力する。
				for indexBuffer in md.meshParts[meshNo].indexBufferArray do (
					--ポリゴンの数を出力する。
					numPoly = indexBuffer.count / 3
					WriteLong file numPoly
					
					
					
					--インデックスバッファを出力。
					for index in indexBuffer do (
						if indexSize == 2 then (
							WriteShort file index
						)else(
							WriteLong file index
						)
					)
					--パディング
					if indexSize == 2 then(
						amari = mod indexBuffer.count 2
						if amari != 0 then(
							WriteShort file 0
						)
					)
				)
			)			
	/*	)catch(
			messageBox "tkmファイルの保存に失敗しました。"
		)*/
		fclose file
	),
public
	fn SaveTkm nodes filePath=
	(
		md = ModelData meshParts:#()

		for p in nodes do (
			--編集可能頂点に変換できるか調べる。
			if canConvertTo p Mesh then(
				--メッシュパーツをビルド				
				BUildMeshParts p
			)
		) 
		--保存。
		InternalSave filePath
	)

)