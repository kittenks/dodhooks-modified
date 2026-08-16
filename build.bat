@echo off
REM ============================================
REM DODHooks - Windows Build Script
REM
REM Builds the extension for Windows x86 and x86_64
REM Requires: Visual Studio 2019+ with C++ tools, Python 3.12+, Git
REM
REM Usage:
REM   build.bat [arch] [sm_branch]
REM   arch: x86, x64, or all (default: all)
REM   sm_branch: 1.12-dev or master (default: 1.12-dev)
REM
REM Example:
REM   build.bat all 1.12-dev
REM   build.bat x64 master
REM ============================================

setlocal EnableDelayedExpansion

set ARCH=%1
if "%ARCH%"=="" set ARCH=all

set SM_BRANCH=%2
if "%SM_BRANCH%"=="" set SM_BRANCH=1.12-dev

set SCRIPT_DIR=%~dp0
set BUILD_ROOT=%SCRIPT_DIR%builds\

echo ============================================
echo  DODHooks Windows Build Script
echo ============================================
echo  SourceMod branch: %SM_BRANCH%
echo  Target arch:      %ARCH%
echo ============================================
echo.

REM --- Install AMBuild if needed ---
echo [1/4] Checking AMBuild...
python -c "import ambuild" 2>nul
if errorlevel 1 (
    echo   Installing AMBuild...
    python -m pip install --upgrade git+https://github.com/alliedmodders/ambuild.git
) else (
    echo   AMBuild already installed.
)

REM --- Clone dependencies ---
echo.
echo [2/4] Cloning dependencies...

if not exist "%SCRIPT_DIR%mmsource\" (
    echo   Cloning Metamod:Source...
    git clone --depth 1 --recurse-submodules -j8 --shallow-submodules -b %SM_BRANCH% https://github.com/alliedmodders/metamod-source.git "%SCRIPT_DIR%mmsource"
) else (
    echo   Metamod:Source exists, updating...
    cd /d "%SCRIPT_DIR%mmsource"
    git pull --ff-only 2>nul || true
    cd /d "%SCRIPT_DIR%"
)

if not exist "%SCRIPT_DIR%sourcemod\" (
    echo   Cloning SourceMod...
    git clone --depth 1 --recurse-submodules -j8 --shallow-submodules -b %SM_BRANCH% https://github.com/alliedmodders/sourcemod.git "%SCRIPT_DIR%sourcemod"
) else (
    echo   SourceMod exists, updating...
    cd /d "%SCRIPT_DIR%sourcemod"
    git pull --ff-only 2>nul || true
    cd /d "%SCRIPT_DIR%"
)

REM --- Build function ---
echo.
echo [3/4] Building extension...

if "%ARCH%"=="all" goto :build_all
if "%ARCH%"=="x86" goto :build_x86
if "%ARCH%"=="x64" goto :build_x64
goto :build_all

:build_all
call :build_x86
call :build_x64
goto :package

:build_x86
echo.
echo ----------------------------------------
echo  Building for x86 (32-bit)
echo ----------------------------------------
if not exist "%SCRIPT_DIR%build_x86\" mkdir "%SCRIPT_DIR%build_x86"
cd /d "%SCRIPT_DIR%build_x86"
python "%SCRIPT_DIR%configure.py" --sm-path "%SCRIPT_DIR%sourcemod" --mms-path "%SCRIPT_DIR%mmsource" --target x86 --enable-optimize
if errorlevel 1 goto :error
ambuild
if errorlevel 1 goto :error
goto :eof

:build_x64
echo.
echo ----------------------------------------
echo  Building for x64 (64-bit)
echo ----------------------------------------
if not exist "%SCRIPT_DIR%build_x64\" mkdir "%SCRIPT_DIR%build_x64"
cd /d "%SCRIPT_DIR%build_x64"
python "%SCRIPT_DIR%configure.py" --sm-path "%SCRIPT_DIR%sourcemod" --mms-path "%SCRIPT_DIR%mmsource" --target x86_64 --enable-optimize
if errorlevel 1 goto :error
ambuild
if errorlevel 1 goto :error
goto :eof

:package
REM --- Package ---
echo.
echo [4/4] Creating release packages...
cd /d "%SCRIPT_DIR%"

REM Copy 32-bit binaries
if exist "%SCRIPT_DIR%build_x86\package\" (
    if not exist "%BUILD_ROOT%1.12\x86\" mkdir "%BUILD_ROOT%1.12\x86\"
    xcopy /E /Y "%SCRIPT_DIR%build_x86\package\addons\sourcemod\extensions\*" "%BUILD_ROOT%1.12\x86\" >nul
)

REM Copy 64-bit binaries
if exist "%SCRIPT_DIR%build_x64\package\" (
    if not exist "%BUILD_ROOT%1.12\x64\" mkdir "%BUILD_ROOT%1.12\x64\"
    xcopy /E /Y "%SCRIPT_DIR%build_x64\package\addons\sourcemod\extensions\*" "%BUILD_ROOT%1.12\x64\" >nul
)

REM Copy gamedata
if exist "%SCRIPT_DIR%sourcemod\gamedata\" (
    if not exist "%BUILD_ROOT%gamedata\" mkdir "%BUILD_ROOT%gamedata\"
    xcopy /E /Y "%SCRIPT_DIR%sourcemod\gamedata\*" "%BUILD_ROOT%gamedata\" >nul
)

echo.
echo ============================================
echo  Build complete!
echo ============================================
echo  Output: %BUILD_ROOT%
echo.
dir /B /S "%BUILD_ROOT%*.dll" 2>nul
dir /B /S "%BUILD_ROOT%*.so" 2>nul
goto :eof

:error
echo.
echo ============================================
echo  BUILD FAILED!
echo ============================================
exit /b 1
