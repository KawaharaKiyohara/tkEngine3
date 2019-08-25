/*!
 * @brief	tkエクスポーターの共通処理。
 */
 
--オブジェクトパラメータ。
struct ObjectParam(
	objName,
	bindPose,
	invBindPose,
	parentId,
	n
)

--オブジェクトパラメータの配列。
g_objectParams = #()

/*!
 * ボーンインデックスを取得する。
 * この関数を呼び出すには、BuildOutputNodeInfoを実行して
 * スケルトン情報を構築済みである必要があります。
*/
fn GetBoneIndex boneName =
(
	boneNo = 0
	for op in g_objectParams do(
		if op.objName == boneName then(
			--見つけた
			return boneNo
		)
		boneNo += 1
	)
)
