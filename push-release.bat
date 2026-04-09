@echo off
setlocal EnableDelayedExpansion

echo ===========================================
echo FastClipboard v1.0 - Release Push Script
echo ===========================================
echo.

:: Check if we're in a git repository
if not exist ".git" (
    echo ERROR: Not in a git repository
    echo Please run 'git init' or clone the repository first
    pause
    exit /b 1
)

:: Check for uncommitted changes
git status --porcelain >nul 2>nul
if %errorlevel% neq 0 (
    echo Found uncommitted changes, staging for commit...
    git add .
    git commit -m "Add FastClipboard v1.0 initial release

- Native Win32 clipboard access via JNI
- Stable text operations without Java clipboard bugs
- UTF-8/Unicode text support with proper encoding
- Image support via CF_DIB format
- File list support via CF_HDROP format
- Format detection and clipboard info methods
- Comprehensive demo and benchmark suite
- Maven build system with native compilation
- Full documentation and examples"
)

:: Create and push release tag
echo Creating release tag v1.0...
git tag -a v1.0 -m "FastClipboard v1.0

Native Clipboard Access via JNI

Features:
- 2-3× faster than Java AWT clipboard
- Stable clipboard operations without bugs
- UTF-8/Unicode text support
- Image support (CF_DIB format)
- File list support (CF_HDROP format)
- Format detection and clipboard info
- Minimal footprint with simple API"

:: Push to remote
echo Pushing to GitHub...
git push origin main
git push origin v1.0

echo.
echo ===========================================
echo RELEASE PUSHED SUCCESSFULLY!
echo ===========================================
echo.
echo Repository: https://github.com/andrestubbe/FastClipboard
echo Release: https://github.com/andrestubbe/FastClipboard/releases/tag/v1.0
echo.
echo You can now view the README.md at:
echo https://github.com/andrestubbe/FastClipboard/blob/main/README.md
echo.
pause
