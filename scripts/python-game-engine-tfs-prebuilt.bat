setlocal EnableDelayedExpansion

if exist ("!VS120COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS120COMNTOOLS!")
	@call ("!VS120COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS110COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS110COMNTOOLS!")
	@call ("!VS110COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS100COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS100COMNTOOLS!")
	@call ("!VS100COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS90COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS90COMNTOOLS!")
	@call ("!VS90COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS80COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS80COMNTOOLS!")
	@call ("!VS80COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS71COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS71COMNTOOLS!")
	@call ("!VS71COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else if exist ("!VS70COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") (
	@echo ("!VS70COMNTOOLS!")
	@call ("!VS70COMNTOOLS!" + "\..\..\VC\vcvarsall.bat") x86_64
) else (
	echo Warning: Could not find environment variables for Visual Studio 64
)
@set CYGWIN_HOME=D:\cygwin64
@set CMAKE_HOME="C:\Program Files (x86)"
@set CMAKE_GEN="Visual Studio 12 2013 Win64"
@set USER_LIBS=D:\user_libs
:parse
IF "%~1"=="" GOTO endparse
IF "%~1"=="--cygwin-path" (
    SHIFT
    @set CYGWIN_HOME="%~1"
)
IF "%~1"=="--cmake-path" (
    SHIFT
    @set CMAKE_HOME="%~1"
)
IF "%~1"=="--user-libs-path" (
    @set USER_LIBS="%~2"
    SHIFT
)
IF "%~1"=="--cmake-visual-studio-gen" (
    @set CMAKE_GEN="%~2"
    SHIFT
)
IF "%~1"=="--help" (
    GOTO help
)
IF "%~1"=="-h" (
    GOTO help
)
SHIFT
GOTO parse
:endparse
@set CYGWIN_BIN=!CYGWIN_HOME!\bin
@set TFS_CYGWIN_HOME=!CYGWIN_HOME!\home\TFSBUILD
@set CMAKE_EXE="C:\Program Files (x86)\CMake\bin\cmake.exe"
@set PATH=!CYGWIN_BIN!;!PATH!
@C:
@chdir !TF_BUILD_BUILDDIRECTORY!
if not exist log @mkdir log 
@chdir src
@!CYGWIN_BIN!\bash.exe scripts\init_git_submodules.sh >../log/init_git_submodules.log 2>&1
if errorlevel 1 (
    chdir ..
    GOTO exit_error
)
@chdir ..
if exist build rd /s /q build
@mkdir build
@chdir build
if not exist CMakeCache.txt !CMAKE_EXE! -DCMAKE_PREFIX_PATH=!USER_LIBS! -DCMAKE_BUILD_TYPE=MinSizeRel ..\src
if errorlevel 1 (
    @chdir ..
    GOTO exit_error
)
GOTO exit
:help
@echo Usage: python-game-engine-tfs-prebuild.bat [-h] [--help] [--cmake-path PATH] [--cygwin-path PATH] [--user-libs-path PATH]
@echo 
@echo Create project from cmake source in build directory.
@echo      -h --help:                   This help.
@echo      --cmake-path:                CMake isntallation path.
@echo      --cygwin-path:               Cygwin isntallation path.
@echo      --user-libs-path:            Path with libraries to find by cmake.
@echo      --cmake-visual-studio-gen:   Visual Studio generator for CMake.

:exit
EXIT /b 0
:exit_error
if exist build @rd /s /q build
EXIT /b 1
