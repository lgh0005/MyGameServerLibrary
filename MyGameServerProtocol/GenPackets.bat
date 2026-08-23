@echo off
setlocal enabledelayedexpansion

:: 1. 인자로 넘겨받은 SolutionDir
set SOLUTION_DIR=%~1
set PROTOC_EXE=%SOLUTION_DIR%ThirdParty\bin\protoc.exe
set PROTO_DIR=%SOLUTION_DIR%MyGameServerProtocol

:: 2. 프로토콜 폴더로 작업 디렉터리 이동
cd /d "%PROTO_DIR%"

:: 3. .proto 파일 타임스탬프 비교 및 컴파일
for %%F in (*.proto) do (
    set TARGET_CC=%%~nF.pb.cc
    set NEED_COMPILE=0

    if not exist "!TARGET_CC!" (
        set NEED_COMPILE=1
    ) else (
        powershell -NoProfile -Command "if ((Get-Item '%%F').LastWriteTime -gt (Get-Item '!TARGET_CC!').LastWriteTime) { exit 1 } else { exit 0 }"
        if errorlevel 1 set NEED_COMPILE=1
    )

    if "!NEED_COMPILE!"=="1" (
        "%PROTOC_EXE%" -I="." --cpp_out="." "%%F"
    )
)