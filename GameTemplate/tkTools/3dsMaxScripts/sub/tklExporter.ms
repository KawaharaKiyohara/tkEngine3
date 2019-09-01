--レベルデータのエクスポータ。

/*
	 * レベルデータを保存。
	 */
     fn SaveLevel = 
     (
         filepath = getSaveFileName caption:"" types: "level (*.tkl)|*.tkl|All Files (*.*)|*.*|"
         
         if filepath != undefined then
         (
             
             file = fopen filepath "wb"
             
             try(
                 --ヘッダーを出力
                 WriteLong file g_objectParams.count
                 
                 for o in g_objectParams do (
                     
                     WriteByte file o.objName.count
                     WriteString file o.objName
                     WriteLong file o.parentId
                     --バインドポーズを出力。
                     for i = 1 to 4 do(
                         WriteFloat file o.bindPose[i].x
                         WriteFloat file o.bindPose[i].y
                         WriteFloat file o.bindPose[i].z
                     )
                     --バインドポーズの逆数を出力。
                     for i = 1 to 4 do(
                         WriteFloat file o.invBindPose[i].x
                         WriteFloat file o.invBindPose[i].y
                         WriteFloat file o.invBindPose[i].z
                     )
                 )
             )catch(
                 Messagebox "ファイルの保存に失敗しました"
             )
             FClose file
         )
     )