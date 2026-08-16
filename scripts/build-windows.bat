@echo off
REM ============================================================
REM DoDHooks Windows Build Script
REM Usage: scripts\build-windows.bat [x86^|x86_64^|all]
REM Requirements:
REM   - Visual Studio 2019 or 2022 with C++ tools
REM   - Python 3.11
REM   - AMBuild 2.2.0 (pip install "AMBuild==2.2.0")
REM   - Git
REM
REM Environment variables (set these before running):
REM   SM_ROOT     - Path to SourceMod source
REM   MM_ROOT     - Path to Metamod:Source source
REM   HL2SDK_ROOT - Parent directory of hl2sdk-dods
REM ============================================================

setlocal EnableDelayedExpansion

set "SCRIPT_DIR=%~dp0"
set "PROJECT_ROOT=%SCRIPT_DIR%.."
set "TARGET=%~1"
if "%TARGET%"=="" set "TARGET=x86_64"

REM ============================================================
REM Default paths (override via environment variables)
REM ============================================================
if "%SM_ROOT%"==""     set "SM_ROOT=%USERPROFILE%\sourcemod"
if "%MM_ROOT%"==""     set "MM_ROOT=%USERPROFILE%\metamod-source"
if "%HL2SDK_ROOT%"=="" set "HL2SDK_ROOT=%USERPROFILE%"

echo ============================================================
echo  DoDHooks Windows Build Script
echo  Project root: %PROJECT_ROOT%
echo  Target: %TARGET%
echo ============================================================
echo.

REM ============================================================
REM Check dependencies
REM ============================================================
echo [*] Checking dependencies...

where python >nul 2>&1
if errorlevel 1 (
    echo [X] python not found in PATH
    echo     Install from https://www.python.org/downloads/
    exit /b 1
)
echo [✓] python found

python -c "import ambuild2" 2>nul
if errorlevel 1 (
    echo [!] AMBuild not found. Installing...
    pip install "AMBuild==2.2.0"
    if errorlevel 1 (
        echo [X] Failed to install AMBuild
        exit /b 1
    )
)
echo [✓] AMBuild found

if not exist "%SM_ROOT%" (
    echo [X] SourceMod not found at: %SM_ROOT%
    echo     Set SM_ROOT environment variable
    echo     Or clone: git clone -b 1.12-dev https://github.com/alliedmodders/sourcemod
    exit /b 1
)
echo [✓] SourceMod found: %SM_ROOT%

if not exist "%MM_ROOT%" (
    echo [X] Metamod:Source not found at: %MM_ROOT%
    echo     Set MM_ROOT environment variable
    echo     Or clone: git clone -b 1.12-dev https://github.com/alliedmodders/metamod-source
    exit /b 1
)
echo [✓] Metamod:Source found: %MM_ROOT%

if not exist "%HL2SDK_ROOT%\hl2sdk-dods" (
    echo [X] HL2SDK DoD:S not found at: %HL2SDK_ROOT%\hl2sdk-dods
    echo     Clone with: git clone -b dods https://github.com/alliedmodders/hl2sdk hl2sdk-dods
    exit /b 1
)
echo [✓] HL2SDK DoD:S found: %HL2SDK_ROOT%\hl2sdk-dods
echo.

REM ============================================================
REM Build function
REM ============================================================
call :build_target %TARGET%
if errorlevel 1 exit /b 1

echo.
echo ============================================================
echo  Build complete!
echo ============================================================
goto :eof

:build_target
set "arch=%~1"
echo [*] Building for %arch%...

cd /d "%PROJECT_ROOT%"

REM Clean
if exist "build_%arch%" rmdir /s /q "build_%arch%"
mkdir "build_%arch%"
cd "build_%arch%"

REM Configure
echo [*] Configuring (%arch%)...
python ..\configure.py ^
    --targets=%arch% ^
    --enable-optimize ^
    --sm-path="%SM_ROOT%" ^
    --mms-path="%MM_ROOT%" ^
    --hl2sdk-root="%HL2SDK_ROOT%" ^
    --sdks=dod

if errorlevel 1 (
    echo [X] Configure failed for %arch%
    exit /b 1
)
echo [✓] Configure successful (%arch%)

REM Build
echo [*] Compiling (%arch%)...
ambuild
if errorlevel 1 (
    echo [X] Build failed for %arch%
    exit /b 1
)
echo [✓] Build successful (%arch%)

REM Show output
if exist "package\dodhooks.ext.dll" (
    echo [✓] Artifact: package\dodhooks.ext.dll
    dir "package\dodhooks.ext.dll"
)

cd /d "%PROJECT_ROOT%"
exit /b 0
