@echo off
@rem �T���v���̊e�t�H���_��tkEngine�ւ̃V���{���b�N�����N���쐬����B
@setlocal
@set CURRENT_DIR=%~dp0

pushd %CURRENT_DIR%

@call .\Sample\cleanup.bat
@call .\Sample\setup.bat

@rem maxScript���R�s�[
@copy "%CURRENT_DIR%GameTemplate\tkTools\3dsMaxScripts\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2015\scripts\"
@copy "%CURRENT_DIR%GameTemplate\tkTools\3dsMaxScripts\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2016\scripts\"
@copy "%CURRENT_DIR%GameTemplate\tkTools\3dsMaxScripts\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2017\scripts\"
@copy "%CURRENT_DIR%GameTemplate\tkTools\3dsMaxScripts\tkExporter.ms" "c:\Program Files\Autodesk\3ds Max 2018\scripts\"



@popd
@echo �Z�b�g�A�b�v�I��

@pause