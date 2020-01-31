/*!
 * @brief	tkEngine関係のエクスポーター。
 */

pluginManager.loadClass FBXIMPORTER 
pluginManager.loadClass FbxExporter

include "sub/tkExporterCommon.ms"
include "sub/tksExporter.ms"
include "sub/tkaExporter.ms"
include "sub/tkmExporter.ms"
include "sub/tklExporter.ms"



rollout tkExporter "tkExporter" width:194 height:248
(
	groupBox 'grp5' "Model" pos:[3,13] width:164 height:138 align:#left
	button 'btn_save_fbx' "Save(*.tkm)" pos:[27,33] width:120 height:28 align:#left
	button 'btn_save_selected_fbx' "Save Selected(*.tkm)" pos:[27,79] width:120 height:28 align:#left
	checkbox 'flat_shading' "flat shading" pos:[27,116] width:128 height:29 align:#left

	groupBox 'grp10' "Level" pos:[3,159] width:164 height:211 align:#left
	label 'lbl4' "edit level name" pos:[23,195] width:121 height:16 align:#left
	editText 'levelName' "" pos:[23,216] width:120 height:24 align:#left
	button 'btn_load_locObject' "Load Object" pos:[26,250] width:120 height:28 align:#left
	button 'btn_copy_locObject' "Copy Object" pos:[26,284] width:120 height:28 align:#left
	button 'SaveLoc' "Save Level" pos:[26,320] width:120 height:28 align:#left
	button 'LoadLoc' "Load Level" pos:[26,355] width:120 height:28 align:#left

	groupBox 'grp6' "Animation" pos:[3,393] width:164 height:200 align:#left
	button 'btn_saveSkeleton' "Save Skeleton" pos:[22,425] width:120 height:28 align:#left
	groupBox 'grp1' "Animation clip" pos:[14,471] width:147 height:134 align:#left
	label 'lbl3' "Label" pos:[33,493] width:0 height:0 align:#left
	label 'lbl1' "start frame" pos:[22,500] width:60 height:16 align:#left
	editText 'animClipStartFrame' "" pos:[90,497] width:58 height:21 align:#left
	label 'lbl2' "end frame" pos:[22,531] width:60 height:16 align:#left
	editText 'animClipEndFrame' "" pos:[90,529] width:58 height:21 align:#left
	button 'btn_saveAnimationClip' "Save" pos:[27,566] width:120 height:28 align:#left

	on  tkExporter open do
		(
			levelName.text = "level00"
		)
	
		/*
		 *  スケルトンの出力のボタンが押されたときの処理。
		 */
		on btn_saveSkeleton pressed do
		(
			if selection[1] == undefined then(
				Messagebox "出力するスケルトンのルートのオブジェクトを選択して下さい。。。"
			)else(
				g_objectParams = #()
				--アニメーション時間を0にする。
				animationRange = interval 0 animationRange.end
				slidertime = 0
				at time 0(
					--アニメーション進行中の処理。
					BuildOutputNodeInfo selection[1] -1
					SaveSkeleton()
				)
			)
		)
		/*
		* Levelを保存。
		*/
		on SaveLoc pressed do
		(
			--現在編集中のルートのレベルを取得。
			editLevelObj = getNodeByName levelName.text
			if editLevelObj == undefined then(
				Messagebox "保存するレベルが存在しません。"
			)else(
				select editLevelObj
			
				--スケルトンとして保存。
				g_objectParams = #()
				--アニメーション時間を0にする。
				animationRange = interval 0 animationRange.end
				slidertime = 0
				at time 0(
					--アニメーション進行中の処理。
					BuildOutputNodeInfo selection[1] -1
					SaveLevel()
				)
			)
		)

		fn LoadFbxFile filepath= 
		(
				if filepath != undefined then(
					print filepath
					editLevelObj = getNodeByName levelName.text
					if editLevelObj == undefined then(
						--ダミーを作る。
						editLevelObj = Dummy()
						editLevelObj.name = levelName.text
					)
					
					FBXImporterSetParam "ScaleConversion" false
					FBXImporterSetParam "UpAxis" "Z"
					ImportFile filepath #noPrompt
					importObj = $
			
					filename = getFilenameFile filepath
					
					--全てを編集可能ポリゴンに変換する。
					delObjArray = #()
					for i=1 to $selection.count do
					(
						p = convertToMesh($selection[i]);
						--編集可能ポリゴンに変換できなかった場合は、削除リストに積む。
						if p == undefined then(
							append delObjArray $selection[i]
						)
					)
					--編集可能ポリゴンに変換できなかったオブジェクトを削除。
					for i=1 to delObjArray.count do
					(
						delete delObjArray[i]
					)
					parentBox = box length:1 width:1 height:1
					p = convertToMesh(parentBox)
					importObj.parent = parentBox
					select p
					selectMore $.children
					--結合。
					while  $selection.count>1 do
					(
						attach p $selection[$selection.count];
					)
	
					--ダミーをレベルの下にぽいっとな。
					$selection[1].pivot = Point3 0 0 0
					selection[1].name = filename
					addModifier selection[1] (maxPlugin())	
					$selection[1].parent = editLevelObj
				)

		)

		/*
		* Levelを読み込み。
		*/
		on LoadLoc pressed do
		(
			filepath = getOpenFileName caption:"" types: "tkl (*tkl)|*.tkl|All Files (*.*)|*.*|"
			pos = findString filepath "level"
			modelPath = substring filepath 1 (pos-1)
			modelPath = modelPath + "modelData\\"

			if filepath != undefined then
			(
				file = fopen filepath "rb"
				try(
					--ヘッダーを読み込み。
					paramCount = ReadLong file
					for i = 1 to paramCount do(
						nameCount = ReadByte file
						objectName = ReadString file
						parentId = ReadLong file

						--バインドポーズを読み込み。
						for i = 1 to 4 do(
							bindPos = ReadFloat file 
							bindPos = ReadFloat file 
							bindPos = ReadFloat file
						)
						--バインドポーズの逆行列を読み込み。
						for i = 1 to 4 do(
							bindPos = ReadFloat file 
							bindPos = ReadFloat file 
							bindPos = ReadFloat file
						)
						caster = ReadByte file
						receiver = ReadByte file
						if parentId != -1 then(
							fbxPath = modelPath + objectName  + ".FBX"
							print fbxPath	
							LoadFbxFile(fbxPath)
						)
					)	
				)catch(
					Messagebox "ファイルの読み込みに失敗しました"
					format "*** % ***\n" (getCurrentException())
				)
				FClose file
			)
		)
		
		/*
		 *  アニメーションクリップの保存のボタンが押されたときの処理。
		 */
		on btn_saveAnimationClip pressed do
		(
			--とりあえず全クリップ出力する。
			skeletonRoot = GetSkeletonRoot objects
			
			if skeletonRoot == undefined  then(
				--シーンにスキンモディファイアが見つからなかった。
				if selection[1] == undefined  then(
					Messagebox "スケルトンのルートのオブジェクトの自動判別ができませんでした。\nスケルトンのルートのオブジェクトを選択してアニメーションクリップを保存してください。\n"
					return 0
				)
				skeletonRoot = selection[1]
			)
			g_objectParams = #()
			--アニメーション時間を0にする。
			startFrame = animClipStartFrame.text as integer
			endFrame = animClipEndFrame.text as integer
			animationRange = interval startFrame endFrame
			slidertimer = 0
			animate on(
				--アニメーション進行中の処理。
				BuildOutputNodeInfo skeletonRoot -1
				try(
					SaveAnimationClip()
				)catch(
					messageBox "ファイルの保存に失敗しました"
				)
			)
		)
		
		function selectHierarchy =
		(
			for p in selection do
			(
				if p.children != undefined do
				(
					selectmore p.children
				)
			)
		)
		
		/*
		 *　配置するオブジェクトをコピー
		 */
		on btn_copy_locObject pressed do
		(
			if selection.count == 0  then(
				Messagebox "コピーするオブジェクトを選択してください。"
			)else(
				selectHierarchy()
				parentObj = copy selection[1]
				parentObj.name = selection[1].name
				for obj in 2 to selection.count do (
					cloneObj = copy selection[obj]
					cloneObj.name = selection[obj].name
					cloneObj.parent = parentObj
					parentObj = merge( parentObj cloneObj )
					deselect cloneObj
				)
				select parentObj
			)
		)


		/*
		 * 配置するオブジェクトのロード。
		 */
		 on btn_load_locObject pressed do
		(
			if  levelName.text == "" then(
				Messagebox "編集するレベルの名前を入力して下さい。"
			)else(
				filepath = getOpenFileName caption:"" types: "fbx (*fbx)|*.fbx|All Files (*.*)|*.*|"
				LoadFbxFile(filepath)
			)
		)

		/*
		 *  FBX出力用の設定を行う。
		 */
		fn SetupFbxExportSettings = 
		(
			--他にもオプションが必要なら追加していってください。
			FBXExporterSetParam "ResetExport"
			FBXExporterSetParam "ScaleFactor" 1.0
			FBXExporterSetParam "UpAxis" "Z"
			FBXExporterSetParam "TangentSpaceExport" true
			FBXExporterSetParam "Triangulate" true
			FBXExporterSetParam "ASCII" true
		)
		
		
		/*!
		 * @brief	FBXのSaveのボタンが押されたときの処理。
		 */
		on btn_save_fbx pressed do
		(
			--全オブジェクトをtkmファイルとして出力する。
			expoter = TkmExporter()
			expoter.SaveTkm objects filepath flat_shading.checked
		)
		
		/*!
		 * @brief	FBXのSave Selectedのボタンが押されたときの処理。
		 */
		on btn_save_selected_fbx pressed do
		(
			--選択されているオブジェクトをtkmファイルとして出力する。
			expoter = TkmExporter()
			expoter.SaveTkm selection filepath flat_shading.checked
		)
	)
	addrollout tkExporter style:#(#style_titlebar, #style_sysmenu)