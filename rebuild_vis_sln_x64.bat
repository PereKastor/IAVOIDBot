
@echo off

cls

set pauseOpt=withpause

if not "%1" == "" (
    set pauseOpt=%1
)


echo pauseOpt=%pauseOpt%

@echo off
if NOT %pauseOpt% == nopause (
    call CheckInstall.bat

    if ErrorLevel == 1 (
        goto fail
    )
)


SET bDirVar=build-solution-x64

echo Removing Old Build Directory

rm -rf %bDirVar% /Q /S

mkdir %bDirVar%

cd %bDirVar%

cmake .. -G "Visual Studio 14 2015 Win64"

cd ..

if NOT %pauseOpt% == nopause (
    pause
)

goto end:

:fail
    echo.
    echo Rebuild FAILED
    goto end

:end