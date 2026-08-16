@echo off
REM ============================================================
REM DoDHooks - Windows Build Script
REM For SourceMod 1.12 / 1.13
REM Supports: x86 (32-bit) and x86_64 (64-bit)
REM ============================================================
setlocal EnableDelayedExpansion

REM ---- Configuration ----
set SM_BRANCH=1.12-dev
set MMS_BRANCH=1.12-dev
set HL2SDK_BRANCH=master

REM ---- Parse arguments ----
set TARGET=x86
if not "%~1"=="" set TARGET=%~1

echo ============================================================
echo  DoDHooks Windows Build Script
echo  Target: %TARGET%
echo ============================================================
echo.

REM ---- Check Python ----
where python >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python not found in PATH
    echo Please install Python 3.8+ and add to PATH
    exit /b 1
)

REM ---- Check Git ----
where git >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Git not found in PATH
    echo Please install Git and add to PATH
    exit /b 1
)

REM ---- Install AMBuild if needed ----
python -c "import ambuild" 2>nul
if errorlevel 1 (
    echo [INFO] Installing AMBuild...
    python -m pip install --upgrade pip
    python -m pip install ambuild
)

REM ---- Setup directories ----
set ROOT=%CD%
set DEPS=%ROOT%\deps
set BUILD=%ROOT%\build-%TARGET%

if not exist "%DEPS%" mkdir "%DEPS%"
if exist "%BUILD%" rmdir /s /q "%BUILD%"
mkdir "%BUILD%"

cd /d "%DEPS%"

REM ---- Clone SourceMod ----
if not exist "sourcemod" (
    echo [INFO] Cloning SourceMod...
    git clone --depth 1 --branch %SM_BRANCH% https://github.com/alliedmodders/sourcemod.git
) else (
    echo [INFO] SourceMod already cloned, updating...
    cd sourcemod
    git fetch --depth 1 origin %SM_BRANCH%
    git checkout %SM_BRANCH%
    git pull origin %SM_BRANCH%
    cd ..
)

REM ---- Clone Metamod:Source ----
if not exist "metamod-source" (
    echo [INFO] Cloning Metamod:Source...
    git clone --depth 1 --branch %MMS_BRANCH% https://github.com/alliedmodders/metamod-source.git
) else (
    echo [INFO] Metamod:Source already cloned, updating...
    cd metamod-source
    git fetch --depth 1 origin %MMS_BRANCH%
    git checkout %MMS_BRANCH%
    git pull origin %MMS_BRANCH%
    cd ..
)

REM ---- Clone HL2SDK for DoD:S ----
if not exist "hl2sdk-dods" (
    echo [INFO] Cloning HL2SDK...
    git clone --depth 1 --branch %HL2SDK_BRANCH% https://github.com/alliedmodders/hl2sdk.git hl2sdk-dods
) else (
    echo [INFO] HL2SDK already cloned, updating...
    cd hl2sdk-dods
    git fetch --depth 1 origin %HL2SDK_BRANCH%
    git checkout %HL2SDK_BRANCH%
    git pull origin %HL2SDK_BRANCH%
    cd ..
)

cd /d "%BUILD%"

REM ---- Configure ----
echo.
echo [INFO] Configuring build for %TARGET%...
python "%ROOT%\configure.py" ^
    --sm-path="%DEPS%\sourcemod" ^
    --mms-path="%DEPS%\metamod-source" ^
    --hl2sdk-root="%DEPS%" ^
    --enable-optimize ^
    --targets=%TARGET%

if errorlevel 1 (
    echo [ERROR] Configuration failed
    exit /b 1
)

REM ---- Build ----
echo.
echo [INFO] Building...
ambuild

if errorlevel 1 (
    echo [ERROR] Build failed
    exit /b 1
)

REM ---- Package ----
echo.
echo [INFO] Packaging...
set PKG_DIR=%BUILD%\dodhooks-%TARGET%
if exist "%PKG_DIR%" rmdir /s /q "%PKG_DIR%"

mkdir "%PKG_DIR%\addons\sourcemod\extensions"
mkdir "%PKG_DIR%\addons\sourcemod\gamedata"
mkdir "%PKG_DIR%\addons\sourcemod\scripting\include"

REM Copy extension binary
for %%f in (package\dodhooks.ext.*) do (
    copy "%%f" "%PKG_DIR%\addons\sourcemod\extensions\" >nul
)

REM Copy gamedata
copy "%ROOT%\sourcemod\gamedata\dodhooks.txt" "%PKG_DIR%\addons\sourcemod\gamedata\" >nul

REM Copy include
copy "%ROOT%\sourcemod\scripting\include\dodhooks.inc" "%PKG_DIR%\addons\sourcemod\scripting\include\" >nul

REM Create autoload
type nul > "%PKG_DIR%\addons\sourcemod\extensions\dodhooks.autoload"

REM Create version info
echo DoDHooks Extension > "%PKG_DIR%\VERSION.txt"
echo Build Date: %date% >> "%PKG_DIR%\VERSION.txt"
echo Architecture: %TARGET% >> "%PKG_DIR%\VERSION.txt"
echo Platform: Windows >> "%PKG_DIR%\VERSION.txt"

REM Create ZIP
cd "%PKG_DIR%"
powershell -command "Compress-Archive -Path * -DestinationPath '%ROOT%\dodhooks-windows-%TARGET%.zip' -Force"

echo.
echo ============================================================
echo  BUILD COMPLETE
echo ============================================================
echo  Output: dodhooks-windows-%TARGET%.zip
echo ============================================================

endlocal
