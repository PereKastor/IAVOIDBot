@echo off

set buildTargetArg=%1

echo You are about to create a job and send it to the AIBootCamp game server
echo.
pause
echo.
echo.

REM CLEAN VARIABLES

set sevenZipPath=
set GOOGLE_DRIVE_DIR=
set PrerequisitesFound=
set googleDriveWellConfigured=
set TEAMNAME=
set filelist=

@echo off
call CheckInstall.bat

if ErrorLevel == 1 (
    goto fail
)

call :Clean
if ErrorLevel 1 (
    goto fail
)

call :RetrieveGoogleDrivePath
call :RetrieveTeamName
call :RetrieveBuildTarget
call :GenerateJobFilename

call :CreateJob
if ErrorLevel 1 (
    goto fail
)

call :SendJobToServer
if ErrorLevel 1 (
    goto fail
)

goto success


:GenerateJobFilename
    set hour=%time:~0,2%
    if "%hour:~0,1%" == " " set hour=0%hour:~1,1%
    set min=%time:~3,2%
    if "%min:~0,1%" == " " set min=0%min:~1,1%
    set secs=%time:~6,2%
    if "%secs:~0,1%" == " " set secs=0%secs:~1,1%

    set JOBNAME=%TEAMNAME%_%DATE%_%hour%-%min%-%secs%_%TARGET%.job

    exit /B 0

:RetrieveTeamName
    set TEAMDRIVE=%GOOGLE_DRIVE_DIR%\Results\Team.txt
    IF EXIST "%TEAMDRIVE%" (
        FOR /F "usebackq delims== tokens=1,2" %%G IN ("%TEAMDRIVE%") DO set %%G=%%H
    )
    exit /B 0

:RetrieveGoogleDrivePath
    IF EXIST "TeamInfo.team" (
        for /f "delims== tokens=1,2" %%G in (TeamInfo.team) do set %%G=%%H
    )
    exit /B 0

:RetrieveBuildTarget
    
    set TARGET=RELEASE
    if "%buildTargetArg%" == "DEBUG" (
        set TARGET=DEBUG
    )
    if "%buildTargetArg%" == "debug" (
        set TARGET=DEBUG
    )

    echo Retrieving Build Target...Success  [%TARGET%]

    exit /B 0

:Clean
    echo.
    if exist *.job (
        del *.job 
        timeout 1 >NUL
    )

    if exist *.job ( 
        echo Cleaning...................Failed
        exit /B 1
    ) else (
        echo Cleaning..................Success
        exit /B 0
    )

:CreateJob

    IF EXIST "jobfiles.txt" (

        for /f %%i in (jobfiles.txt) do set c=%%i
        echo %c%

        echo " " > jobfiles7z.txt
        for %%x in (%c::= %) do echo %%x >> jobfiles7z.txt

        7z a -tzip %JOBNAME% @jobfiles7z.txt
        rm jobfiles7z.txt
    )

    if exist %JOBNAME% (
        echo Creating job file.........Success  [%JOBNAME%]
        exit /B 0
    ) else (
        echo Creating job file.........Failed
        exit /B 1
    )

:SendJobToServer
    copy "%JOBNAME%" "%GOOGLE_DRIVE_DIR%"\jobs >nul

    if exist "%GOOGLE_DRIVE_DIR%\jobs\%JOBNAME%" (
        echo Sending job file..........Success
        exit /B 0
    ) else (
        echo Sending job file...........Failed
        exit /B 1
    )

:success
    echo.
    echo SUCCESS : Job queued
    goto end

:fail
    echo.
    echo FAILED : Job creation process failed
    goto end

:end