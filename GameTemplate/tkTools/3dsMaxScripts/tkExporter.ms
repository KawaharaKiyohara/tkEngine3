/*!
 * @brief	tkEngine?ｿｽﾖ係?ｿｽﾌエ?ｿｽN?ｿｽX?ｿｽ|?ｿｽ[?ｿｽ^?ｿｽ[?ｿｽB
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
	editText 'animClipStartFrame' "" pos:[90,462] width:58 height:21 align:#left
	label 'lbl1' "start frame" pos:[22,465] width:60 height:16 align:#left
	groupBox 'grp1' "Animation clip" pos:[14,436] width:147 height:134 align:#left
	button 'btn_saveAnimationClip' "Save" pos:[27,531] width:120 height:28 align:#left
	label 'lbl2' "end frame" pos:[22,496] width:60 height:16 align:#left
	editText 'animClipEndFrame' "" pos:[90,494] width:58 height:21 align:#left
	button 'btn_saveSkeleton' "Save Skeleton" pos:[22,399] width:120 height:28 align:#left
	label 'lbl3' "Label" pos:[33,458] width:0 height:0 align:#left
	groupBox 'grp5' "Model" pos:[3,13] width:164 height:138 align:#left
	button 'btn_save_fbx' "Save(*.tkm)" pos:[27,33] width:120 height:28 align:#left
	groupBox 'grp10' "Level" pos:[3,159] width:164 height:211 align:#left
	button 'btn_load_locObject' "Load Object" pos:[26,250] width:120 height:28 align:#left
	editText 'levelName' "" pos:[23,216] width:120 height:24 align:#left
	label 'lbl4' "edit level name" pos:[23,195] width:121 height:16 align:#left
	button 'SaveLoc' "Save Level" pos:[26,320] width:120 height:28 align:#left
	groupBox 'grp6' "Animation" pos:[3,373] width:164 height:200 align:#left
	button 'btn_copy_locObject' "Copy Object" pos:[26,284] width:120 height:28 align:#left
	button 'btn_save_selected_fbx' "Save Selected(*.tkm)" pos:[27,79] width:120 height:28 align:#left
	checkbox 'flat_shading' "flat shading" pos:[27,116] width:128 height:29 align:#left
	
	on  tkExporter open do
	(
		levelName.text = "level00"
	)

	/*
	 *  ?ｿｽX?ｿｽP?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾌ出?ｿｽﾍのボ?ｿｽ^?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽﾆゑｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
	 */
	on btn_saveSkeleton pressed do
	(
		if selection[1] == undefined then(
			Messagebox "?ｿｽo?ｿｽﾍゑｿｽ?ｿｽ?ｿｽX?ｿｽP?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ[?ｿｽg?ｿｽﾌオ?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽI?ｿｽ?ｿｽﾄ会ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB?ｿｽB?ｿｽB"
		)else(
			g_objectParams = #()
			--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾔゑｿｽ0?ｿｽﾉゑｿｽ?ｿｽ?ｿｽB
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽi?ｿｽs?ｿｽ?ｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
				BuildOutputNodeInfo selection[1] -1
				SaveSkeleton()
			)
		)
	)
	/*
	* Level?ｿｽ?ｿｽﾛ托ｿｽ?ｿｽB
	*/
	on SaveLoc pressed do
	(
		--?ｿｽ?ｿｽ?ｿｽﾝ編集?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ[?ｿｽg?ｿｽﾌ??ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ謫ｾ?ｿｽB
		editLevelObj = getNodeByName levelName.text
		if editLevelObj == undefined then(
			Messagebox "?ｿｽﾛ托ｿｽ?ｿｽ?ｿｽ?ｿｽ驛鯉ｿｽx?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾝゑｿｽ?ｿｽﾜゑｿｽ?ｿｽ?ｿｽB"
		)else(
			select editLevelObj
		
			--?ｿｽX?ｿｽP?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾆゑｿｽ?ｿｽﾄ保托ｿｽ?ｿｽB
			g_objectParams = #()
			--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾔゑｿｽ0?ｿｽﾉゑｿｽ?ｿｽ?ｿｽB
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽi?ｿｽs?ｿｽ?ｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
				BuildOutputNodeInfo selection[1] -1
				SaveLevel()
				print "save level"
			)
		)
	)
	
	/*
	 *  ?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽv?ｿｽﾌ保托ｿｽ?ｿｽﾌボ?ｿｽ^?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽﾆゑｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
	 */
	on btn_saveAnimationClip pressed do
	(
		--?ｿｽﾆりあ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽS?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽv?ｿｽo?ｿｽﾍゑｿｽ?ｿｽ?ｿｽB
		skeletonRoot = GetSkeletonRoot objects
		
		if skeletonRoot == undefined  then(
			--?ｿｽV?ｿｽ[?ｿｽ?ｿｽ?ｿｽﾉス?ｿｽL?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽf?ｿｽB?ｿｽt?ｿｽ@?ｿｽC?ｿｽA?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾂゑｿｽ?ｿｽ?ｿｽﾈゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB
			if selection[1] == undefined  then(
				Messagebox "?ｿｽX?ｿｽP?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ[?ｿｽg?ｿｽﾌオ?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽﾌ趣ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾊゑｿｽ?ｿｽﾅゑｿｽ?ｿｽﾜゑｿｽ?ｿｽ?ｿｽﾅゑｿｽ?ｿｽ?ｿｽ?ｿｽB\n?ｿｽX?ｿｽP?ｿｽ?ｿｽ?ｿｽg?ｿｽ?ｿｽ?ｿｽﾌ??ｿｽ?ｿｽ[?ｿｽg?ｿｽﾌオ?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽI?ｿｽ?ｿｽﾄア?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽN?ｿｽ?ｿｽ?ｿｽb?ｿｽv?ｿｽ?ｿｽﾛ托ｿｽ?ｿｽ?ｿｽ?ｿｽﾄゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB\n"
				return 0
			)
			skeletonRoot = selection[1]
		)
		g_objectParams = #()
		--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽﾔゑｿｽ0?ｿｽﾉゑｿｽ?ｿｽ?ｿｽB
		startFrame = animClipStartFrame.text as integer
		endFrame = animClipEndFrame.text as integer
		animationRange = interval startFrame endFrame
		slidertimer = 0
		animate on(
			--?ｿｽA?ｿｽj?ｿｽ?ｿｽ?ｿｽ[?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽi?ｿｽs?ｿｽ?ｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
			BuildOutputNodeInfo skeletonRoot -1
			try(
				SaveAnimationClip()
			)catch(
				messageBox "?ｿｽt?ｿｽ@?ｿｽC?ｿｽ?ｿｽ?ｿｽﾌ保托ｿｽ?ｿｽﾉ趣ｿｽ?ｿｽs?ｿｽ?ｿｽ?ｿｽﾜゑｿｽ?ｿｽ?ｿｽ"
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
	 *?ｿｽ@?ｿｽz?ｿｽu?ｿｽ?ｿｽ?ｿｽ?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽR?ｿｽs?ｿｽ[
	 */
	on btn_copy_locObject pressed do
	(
		if selection.count == 0  then(
			Messagebox "?ｿｽR?ｿｽs?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽI?ｿｽ?ｿｽﾄゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB"
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
	 * ?ｿｽz?ｿｽu?ｿｽ?ｿｽ?ｿｽ?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽﾌ??ｿｽ?ｿｽ[?ｿｽh?ｿｽB
	 */
	on btn_load_locObject pressed do
	(
		
		if  levelName.text == "" then(
			Messagebox "?ｿｽﾒ集?ｿｽ?ｿｽ?ｿｽ驛鯉ｿｽx?ｿｽ?ｿｽ?ｿｽﾌ厄ｿｽ?ｿｽO?ｿｽ?ｿｽ?ｿｽ?ｿｽﾍゑｿｽ?ｿｽﾄ会ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB"
		)else(
			filepath = getOpenFileName caption:"" types: "fbx (*fbx)|*.fbx|All Files (*.*)|*.*|"
			if filepath != undefined then(
				editLevelObj = getNodeByName levelName.text
				if editLevelObj == undefined then(
					--?ｿｽ_?ｿｽ~?ｿｽ[?ｿｽ?ｿｽ?ｿｽ?ｿｽB
					editLevelObj = Dummy()
					editLevelObj.name = levelName.text
				)
				
				FBXImporterSetParam "ScaleConversion" false
				FBXImporterSetParam "UpAxis" "Z"
				ImportFile filepath #noPrompt
				importObj = $
		
				filename = getFilenameFile filepath
				
				--?ｿｽS?ｿｽﾄゑｿｽﾒ集?ｿｽﾂ能?ｿｽ|?ｿｽ?ｿｽ?ｿｽS?ｿｽ?ｿｽ?ｿｽﾉ変奇ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB
				delObjArray = #()
				for i=1 to $selection.count do
				(
					p = convertToMesh($selection[i]);
					--?ｿｽﾒ集?ｿｽﾂ能?ｿｽ|?ｿｽ?ｿｽ?ｿｽS?ｿｽ?ｿｽ?ｿｽﾉ変奇ｿｽ?ｿｽﾅゑｿｽ?ｿｽﾈゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ鼾??ｿｽﾍ、?ｿｽ尞懶ｿｽ?ｿｽ?ｿｽX?ｿｽg?ｿｽﾉ積む。
					if p == undefined then(
						append delObjArray $selection[i]
					)
				)
				--?ｿｽﾒ集?ｿｽﾂ能?ｿｽ|?ｿｽ?ｿｽ?ｿｽS?ｿｽ?ｿｽ?ｿｽﾉ変奇ｿｽ?ｿｽﾅゑｿｽ?ｿｽﾈゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽ尞懶ｿｽB
				for i=1 to delObjArray.count do
				(
					delete delObjArray[i]
				)
				parentBox = box length:1 width:1 height:1
				p = convertToMesh(parentBox)
				importObj.parent = parentBox
				select p
				selectMore $.children
				--?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB
				while  $selection.count>1 do
				(
					attach p $selection[$selection.count];
				)

				--?ｿｽ_?ｿｽ~?ｿｽ[?ｿｽ?ｿｽx?ｿｽ?ｿｽ?ｿｽﾌ会ｿｽ?ｿｽﾉぽゑｿｽ?ｿｽ?ｿｽ?ｿｽﾆな。
				$selection[1].pivot = Point3 0 0 0
				selection[1].name = filename
				addModifier selection[1] (maxPlugin())	
				$selection[1].parent = editLevelObj
			)
		)
	)
	/*
	 *  FBX?ｿｽo?ｿｽﾍ用?ｿｽﾌ設抵ｿｽ?ｿｽs?ｿｽ?ｿｽ?ｿｽB
	 */
	fn SetupFbxExportSettings = 
	(
		--?ｿｽ?ｿｽ?ｿｽﾉゑｿｽI?ｿｽv?ｿｽV?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽK?ｿｽv?ｿｽﾈゑｿｽﾇ会ｿｽ?ｿｽ?ｿｽ?ｿｽﾄゑｿｽ?ｿｽ?ｿｽ?ｿｽﾄゑｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽB
		FBXExporterSetParam "ResetExport"
		FBXExporterSetParam "ScaleFactor" 1.0
		FBXExporterSetParam "UpAxis" "Z"
		FBXExporterSetParam "TangentSpaceExport" true
		FBXExporterSetParam "Triangulate" true
		FBXExporterSetParam "ASCII" true
	)
	
	
	/*!
	 * @brief	FBX?ｿｽ?ｿｽSave?ｿｽﾌボ?ｿｽ^?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽﾆゑｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
	 */
	on btn_save_fbx pressed do
	(
		--?ｿｽS?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽtkm?ｿｽt?ｿｽ@?ｿｽC?ｿｽ?ｿｽ?ｿｽﾆゑｿｽ?ｿｽﾄ出?ｿｽﾍゑｿｽ?ｿｽ?ｿｽB
		expoter = TkmExporter()
		expoter.SaveTkm objects filepath flat_shading.checked
	)
	
	/*!
	 * @brief	FBX?ｿｽ?ｿｽSave Selected?ｿｽﾌボ?ｿｽ^?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ?ｿｽ黷ｽ?ｿｽﾆゑｿｽ?ｿｽﾌ擾ｿｽ?ｿｽ?ｿｽ?ｿｽB
	 */
	on btn_save_selected_fbx pressed do
	(
		--?ｿｽI?ｿｽ?ｿｽ?ｿｽﾄゑｿｽ?ｿｽ?ｿｽI?ｿｽu?ｿｽW?ｿｽF?ｿｽN?ｿｽg?ｿｽ?ｿｽtkm?ｿｽt?ｿｽ@?ｿｽC?ｿｽ?ｿｽ?ｿｽﾆゑｿｽ?ｿｽﾄ出?ｿｽﾍゑｿｽ?ｿｽ?ｿｽB
		expoter = TkmExporter()
		expoter.SaveTkm selection filepath flat_shading.checked
	)
)
addrollout tkExporter style:#(#style_titlebar, #style_sysmenu)