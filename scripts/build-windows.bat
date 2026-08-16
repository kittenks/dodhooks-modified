@echo off
REM DoDHooks Windows 本地编译脚本
REM 用法: build-windows.bat [x86|x86_64]

setlocal EnableDelayedExpansion

set ARCH=%1
if "%ARCH%"=="" set ARCH=x86_64

echo ==========================================
echo   DoDHooks Windows Build
echo   Arch: %ARCH%
echo ==========================================

set ROOT_DIR=%~dp0..
set WORKSPACE=%ROOT_DIR%\..

REM 安装 AMBuild
echo [1/5] Installing AMBuild 2.2.0...
pip install "AMBuild==2.2.0"
if errorlevel 1 (
    echo ERROR: Failed to install AMBuild
    exit /b 1
)

REM 检查/克隆依赖
echo [2/5] Checking dependencies...

if not exist "%WORKSPACE%\sourcemod" (
    echo   Cloning SourceMod...
    git clone --depth 1 -b 1.12-dev https://github.com/alliedmodders/sourcemod.git "%WORKSPACE%\sourcemod"
)

if not exist "%WORKSPACE%\mmsource" (
    echo   Cloning Metamod:Source...
    git clone --depth 1 -b 1.11-dev https://github.com/alliedmodders/metamod-source.git "%WORKSPACE%\mmsource"
)

if not exist "%WORKSPACE%\hl2sdk-dods" (
    echo   Cloning hl2sdk (dods)...
    git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git "%WORKSPACE%\hl2sdk-dods"
)

REM 设置 MSVC 环境
echo [3/5] Setting up MSVC...
if "%ARCH%"=="x86" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)

REM Configure
echo [4/5] Configuring...
cd /d "%ROOT_DIR%"
if exist build rmdir /s /q build
mkdir build
cd build

python ..\configure.py ^
    --targets=%ARCH% ^
    --enable-optimize ^
    --sm-path=%WORKSPACE%\sourcemod ^
    --mms-path=%WORKSPACE%\mmsource ^
    --hl2sdk-root=%WORKSPACE% ^
    --sdks=dod

if errorlevel 1 (
    echo ERROR: Configure failed
    exit /b 1
)

REM Build
echo [5/5] Building...
ambuild
if errorlevel 1 (
    echo ERROR: Build failed
    exit /b 1
)

echo.
echo ==========================================
echo   BUILD SUCCESS
echo   Output: %ROOT_DIR%\build\package\
echo ==========================================
dir "%ROOT_DIR%\build\package\" 2>nul || echo (package dir not found)
