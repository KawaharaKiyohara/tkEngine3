/*!
 * @brief	tk�G�N�X�|�[�^�[�̋��ʏ����B
 */
 
--�I�u�W�F�N�g�p�����[�^�B
struct ObjectParam(
	objName,
	bindPose,
	invBindPose,
	parentId,
	n
)


--�I�u�W�F�N�g�p�����[�^�̔z��B
g_objectParams = #()

/*
*  �o�͂���m�[�h�̏���\�z�B
*/
fn BuildOutputNodeInfo n parentId= 
(
	objParam = ObjectParam "" Matrix3 Matrix3 -1 Node
	objParam.objName = n.name
	objParam.bindPose = n.transform
	objParam.invBindPose = inverse n.transform
	objParam.n = n
	if n.parent == undefined then(
		--�e�����Ȃ�
		objParam.parentId = -1
	)else(
		--�e������
		objParam.parentId = parentId
	)
	append g_objectParams objParam
	parentId = g_objectParams.count-1
	--�q����B
	for child in n.children do(
		BuildOutputNodeInfo child parentId
	)
)

/*!
 * �{�[���C���f�b�N�X��擾����B
 * ���̊֐���Ăяo���ɂ́ABuildOutputNodeInfo����s����
 * �X�P���g������\�z�ς݂ł���K�v������܂��B
*/
fn GetBoneIndex boneName =
(
	boneNo = 0
	for op in g_objectParams do(
		if op.objName == boneName then(
			--������
			return boneNo
		)
		boneNo += 1
	)
)
/*
*  �X�L�����f�B�t�@�C�A���ݒ肳��Ă���m�[�h����������A���̃m�[�h�Ɋ��蓖�Ă��Ă���X�P���g���̃��[�g��擾�B
*/
fn GetSkeletonRoot nodes =
(
	for p in nodes do
	(
		--�X�L�����f�B�t�@�C�A��擾�B
		skinMod = p.modifiers[skin]
		
		if skinMod != undefined then(
			--�X�L��������B
			max modify mode
			
			modPanel.setCurrentObject skinMod
			numBone = skinOps.GetNumberBones( skinMod )
			if numBone > 0 then(
				--bone����������������
				boneName = skinOps.GetBoneName skinMod 1 1 
				--���O����m�[�h��I��
				boneNode = getNodebyName(boneName)
				if boneNode != undefined  then (
					--�X�P���g���̃��[�g������邼
					while boneNode.parent != undefined  do(
						boneNode = boneNode.parent
					)
					--���[�g��Ԃ���B
					return boneNode
				)
			)
		)
	)
	return undefined
)
/*
*  �}�e���A�����\���Ă��Ȃ��m�[�h���Ȃ����`�F�b�N��s���B
*/
fn CheckMaterial nodes =
(
	for p in nodes do
	(
		meshCount = GetTriMeshFaceCount(p)
		if classof( p ) == BoneGeometry do(
			--�����̓{�[��
			meshCount[1] = 0
			meshCount[2] = 0
		)
		if meshCount[1] > 0 do
		(
			--���b�V��������B
			if p.material == undefined do
			(
				--�}�e���A���������Ă��Ȃ��B
				errorMessage = "�G���[\n " + p.name + "�Ƀ}�e���A�������蓖�Ă��Ă��Ȃ����ߏo�͂ł��܂���B\n"+ "�}�e���A����蓖�ĂĂ��������B\n"
				messagebox errorMessage
				return false				
			)
		)
	)
	return true
)