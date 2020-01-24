fn SaveLevel = 
(
    filepath = getSaveFileName caption:"" types: "level (*.tkl)|*.tkl|All Files (*.*)|*.*|"
    
    if filepath != undefined then
    (
        
        file = fopen filepath "wb"
        
        try(
            --?øΩw?øΩb?øΩ_?øΩ[?øΩ?øΩo?øΩ?øΩ
            WriteLong file g_objectParams.count
            
            for o in g_objectParams do (
                
                WriteByte file o.objName.count
                WriteString file o.objName
                WriteLong file o.parentId
                --?øΩo?øΩC?øΩ?øΩ?øΩh?øΩ|?øΩ[?øΩY?øΩ?øΩo?øΩÕÅB
                for i = 1 to 4 do(
                    WriteFloat file o.bindPose[i].x
                    WriteFloat file o.bindPose[i].y
                    WriteFloat file o.bindPose[i].z
                )
                --?øΩo?øΩC?øΩ?øΩ?øΩh?øΩ|?øΩ[?øΩY?øΩÃãt?øΩ?øΩ?øΩ?øΩo?øΩÕÅB
                for i = 1 to 4 do(
                    WriteFloat file o.invBindPose[i].x
                    WriteFloat file o.invBindPose[i].y
                    WriteFloat file o.invBindPose[i].z
                )
            )
        )catch(
            Messagebox "?øΩt?øΩ@?øΩC?øΩ?øΩ?øΩÃï€ëÔøΩ?øΩ…éÔøΩ?øΩs?øΩ?øΩ?øΩ‹ÇÔøΩ?øΩ?øΩ"
        )
        FClose file
    )
)