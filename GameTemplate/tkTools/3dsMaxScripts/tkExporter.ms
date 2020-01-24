/*!
 * @brief	tkEngine�֌W�̃G�N�X�|�[�^�[�B
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
	 *  �X�P���g���̏o�͂̃{�^���������ꂽ�Ƃ��̏����B
	 */
	on btn_saveSkeleton pressed do
	(
		if selection[1] == undefined then(
			Messagebox "�o�͂���X�P���g���̃��[�g�̃I�u�W�F�N�g��I��ĉ������B�B�B"
		)else(
			g_objectParams = #()
			--�A�j���[�V�������Ԃ�0�ɂ���B
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--�A�j���[�V�����i�s���̏����B
				BuildOutputNodeInfo selection[1] -1
				SaveSkeleton()
			)
		)
	)
	/*
	* Level��ۑ��B
	*/
	on SaveLoc pressed do
	(
		--���ݕҏW���̃��[�g�̃��x����擾�B
		editLevelObj = getNodeByName levelName.text
		if editLevelObj == undefined then(
			Messagebox "�ۑ����郌�x�������݂��܂���B"
		)else(
			select editLevelObj
		
			--�X�P���g���Ƃ��ĕۑ��B
			g_objectParams = #()
			--�A�j���[�V�������Ԃ�0�ɂ���B
			animationRange = interval 0 animationRange.end
			slidertime = 0
			at time 0(
				--�A�j���[�V�����i�s���̏����B
				BuildOutputNodeInfo selection[1] -1
				SaveLevel()
				print "save level"
			)
		)
	)
	
	/*
	 *  �A�j���[�V�����N���b�v�̕ۑ��̃{�^���������ꂽ�Ƃ��̏����B
	 */
	on btn_saveAnimationClip pressed do
	(
		--�Ƃ肠�����S�N���b�v�o�͂���B
		skeletonRoot = GetSkeletonRoot objects
		
		if skeletonRoot == undefined  then(
			--�V�[���ɃX�L�����f�B�t�@�C�A��������Ȃ������B
			if selection[1] == undefined  then(
				Messagebox "�X�P���g���̃��[�g�̃I�u�W�F�N�g�̎������ʂ��ł��܂���ł����B\n�X�P���g���̃��[�g�̃I�u�W�F�N�g��I��ăA�j���[�V�����N���b�v��ۑ����Ă��������B\n"
				return 0
			)
			skeletonRoot = selection[1]
		)
		g_objectParams = #()
		--�A�j���[�V�������Ԃ�0�ɂ���B
		startFrame = animClipStartFrame.text as integer
		endFrame = animClipEndFrame.text as integer
		animationRange = interval startFrame endFrame
		slidertimer = 0
		animate on(
			--�A�j���[�V�����i�s���̏����B
			BuildOutputNodeInfo skeletonRoot -1
			try(
				SaveAnimationClip()
			)catch(
				messageBox "�t�@�C���̕ۑ��Ɏ��s���܂���"
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
	 *�@�z�u����I�u�W�F�N�g��R�s�[
	 */
	on btn_copy_locObject pressed do
	(
		if selection.count == 0  then(
			Messagebox "�R�s�[����I�u�W�F�N�g��I��Ă��������B"
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
	 * �z�u����I�u�W�F�N�g�̃��[�h�B
	 */
	on btn_load_locObject pressed do
	(
		
		if  levelName.text == "" then(
			Messagebox "�ҏW���郌�x���̖��O����͂��ĉ������B"
		)else(
			filepath = getOpenFileName caption:"" types: "fbx (*fbx)|*.fbx|All Files (*.*)|*.*|"
			if filepath != undefined then(
				editLevelObj = getNodeByName levelName.text
				if editLevelObj == undefined then(
					--�_�~�[����B
					editLevelObj = Dummy()
					editLevelObj.name = levelName.text
				)
				
				FBXImporterSetParam "ScaleConversion" false
				FBXImporterSetParam "UpAxis" "Z"
				ImportFile filepath #noPrompt
				importObj = $
		
				filename = getFilenameFile filepath
				
				--�S�Ă�ҏW�\�|���S���ɕϊ�����B
				delObjArray = #()
				for i=1 to $selection.count do
				(
					p = convertToMesh($selection[i]);
					--�ҏW�\�|���S���ɕϊ��ł��Ȃ������ꍇ�́A�폜���X�g�ɐςށB
					if p == undefined then(
						append delObjArray $selection[i]
					)
				)
				--�ҏW�\�|���S���ɕϊ��ł��Ȃ������I�u�W�F�N�g��폜�B
				for i=1 to delObjArray.count do
				(
					delete delObjArray[i]
				)
				parentBox = box length:1 width:1 height:1
				p = convertToMesh(parentBox)
				importObj.parent = parentBox
				select p
				selectMore $.children
				--�����B
				while  $selection.count>1 do
				(
					attach p $selection[$selection.count];
				)

				--�_�~�[��x���̉��ɂۂ����ƂȁB
				$selection[1].pivot = Point3 0 0 0
				selection[1].name = filename
				--addModifier selection[1] (maxPlugin())	
				$selection[1].parent = editLevelObj
			)
		)
	)
	/*
	 *  FBX�o�͗p�̐ݒ��s���B
	 */
	fn SetupFbxExportSettings = 
	(
		--���ɂ�I�v�V�������K�v�Ȃ�ǉ����Ă����Ă��������B
		FBXExporterSetParam "ResetExport"
		FBXExporterSetParam "ScaleFactor" 1.0
		FBXExporterSetParam "UpAxis" "Z"
		FBXExporterSetParam "TangentSpaceExport" true
		FBXExporterSetParam "Triangulate" true
		FBXExporterSetParam "ASCII" true
	)
	
	
	/*!
	 * @brief	FBX��Save�̃{�^���������ꂽ�Ƃ��̏����B
	 */
	on btn_save_fbx pressed do
	(
		--�S�I�u�W�F�N�g��tkm�t�@�C���Ƃ��ďo�͂���B
		expoter = TkmExporter()
		expoter.SaveTkm objects filepath flat_shading.checked
	)
	
	/*!
	 * @brief	FBX��Save Selected�̃{�^���������ꂽ�Ƃ��̏����B
	 */
	on btn_save_selected_fbx pressed do
	(
		--�I���Ă���I�u�W�F�N�g��tkm�t�@�C���Ƃ��ďo�͂���B
		expoter = TkmExporter()
		expoter.SaveTkm selection filepath flat_shading.checked
	)
)
addrollout tkExporter style:#(#style_titlebar, #style_sysmenu)