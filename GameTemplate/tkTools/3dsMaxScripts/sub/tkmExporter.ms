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
--メッシュパーツ。
struct MeshParts(
	indexBuffer = #(),
	vertexBuffer = #()
)
--モデルデータ。
struct ModelData(
	meshParts = #()
)

fn SaveTkm nodes filePath=
(
	md = ModelData meshParts:#()
	ms = Meshparts indexbuffer:#() vertexBuffer:#()	
	
	
	for p in nodes do (
		--編集可能頂点に変換できるか調べる。
		if canConvertTo p Mesh then(				
			--マテリアルごとにグループ分けを行う。
			if p.material != undefined then(
				--サブマテリアルがある？
				numSubMaterial = getNumSubMtls p.material
				if numSubMaterial < 0 then(
					--サブマテリアルがあるので、マテリアルごとに三角形を分割する。
				)else(
					--集約しないとサブマテリアルＩＤが取れないぞいぞい。
					meshObj = copy p 
					meshObj = convertToMesh meshObj
					numFace = getNumFaces p
					--単一マテリアル。
					for faceNo = 1 to numFace do (
						matId  = getFaceMatID meshObj faceNo
						
						--三角形を構成するインデックスを取得する。
						vertNos = getFace meshObj faceNo
						append ms.indexBuffer vertNos[1]
						append ms.indexBuffer vertNos[2]
						append ms.indexBuffer vertNos[3]
					)
					
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
						skinMod = p.modifiers[skin]
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
					append md.meshParts ms
				)
			)
		)
	) 
	
	--保存だよ
	file = fopen filePath "wb"
	--try(
		--ヘッダーを出力
		WriteShort file TKM_VERSION 
		--メッシュパーツの数を出力
		WriteShort file md.meshParts.count
		--メッシュパーツを出力していく。
		for meshNo = 1 to md.meshParts.count do(
			--ポリゴンの数を出力する。
			numPoly = md.meshParts[meshNo].indexBuffer.count / 3
			WriteLong file numPoly
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
			--インデックスバッファを出力。
			
			for index in md.meshParts[meshNo].indexBuffer do (
				if indexSize == 2 then (
					WriteShort file index
				)else(
					WriteLong file index
				)
			)
			--パディング
			if indexSize == 2 then(
				amari = mod md.meshParts[meshNo].indexBuffer.count 2
				if amari != 0 then(
					WriteShort file 0
				)
			)
		)			
/*	)catch(
		messageBox "tkmファイルの保存に失敗しました。"
	)*/
	fclose file
)
