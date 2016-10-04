@echo off
REM CLEAN VARIABLES

set sevenZipPath=
set GOOGLE_DRIVE_DIR=
set PrerequisitesFound=
set googleDriveWellConfigured=
set TEAMNAME=
set AIBOOTCAMP_ENGINE_DIR=
set AIBOOTCAMP_ENGINE_VERSION=
set PYTHON_PATH=

@echo off
call :ValidatePrerequisites

if ErrorLevel == 1 (
    goto fail
)

goto success

:ValidatePrerequisites
    echo Checking Prerequisites

    set PrerequisitesFound=0

    call :RetrieveGoogleDrivePath

    IF NOT EXIST "TeamInfo.team" (
        echo    TeamInfo.info...............Error 
        set PrerequisitesFound=1
    ) else (
        echo    TeamInfo.info...............Found   ["%~dp0TeamInfo.team"]
    )

    @echo off
    for /f "delims=" %%a in ('where 7z') do @set sevenZipPath=%%a
    IF NOT EXIST "%sevenZipPath%" (
       echo    7z..........................Error
       set PrerequisitesFound=1
    ) else (
       echo    7z..........................Found   ["%sevenZipPath%"]
    )

     @echo off
    IF NOT EXIST "%PYTHON_PATH%" (
      for /f "delims=" %%a in ('where python') do @set PYTHON_PATH=%%a
    )
    IF NOT EXIST "%PYTHON_PATH%" (
       echo    Python......................Error
       set PrerequisitesFound=1
    ) else (
       echo    Python......................Found   ["%PYTHON_PATH%"]
    )

    @echo off
    for /f "delims=" %%a in ('where cmake') do @set cmakePath=%%a
    IF NOT EXIST "%cmakePath%" (
       echo    CMake.......................Error
       set PrerequisitesFound=1
    ) else (
       echo    CMake.......................Found   ["%cmakePath%"]
    )

    set googleDriveWellConfigured=0
    IF EXIST "%GOOGLE_DRIVE_DIR%" (
       IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Results" (
              echo    GOOGLE_DRIVE_DIR.................Error   Results Folder Not Found
              set PrerequisitesFound=1
              set googleDriveWellConfigured=1
       ) ELSE (
            IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Results/Team.txt" (
                  echo    GOOGLE_DRIVE_DIR.................Error   Team.txt File Not Found in Results Folder
                  set PrerequisitesFound=1
                  set googleDriveWellConfigured=1
           )
       )

       IF NOT EXIST "%GOOGLE_DRIVE_DIR%/Jobs" (
              echo    GOOGLE_DRIVE_DIR............Error   Jobs Folder Not Found
              set PrerequisitesFound=1
              set googleDriveWellConfigured=1
       )
    ) ELSE (
       echo    GOOGLE_DRIVE_DIR............Error   Folder Not Found
    )
       
     IF %googleDriveWellConfigured% == 0 (
       echo    GOOGLE_DRIVE_DIR............Found   ["%GOOGLE_DRIVE_DIR%"]
       call :RetrieveTeamName
    ) 

    IF "%TEAMNAME%" == "" (
       echo    TEAMNAME....................Error   Team Name Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    TEAMNAME....................Found   ["%TEAMNAME%"]
    )

    IF "%AIBOOTCAMP_ENGINE_DIR%" == "" (
       echo    AIBOOTCAMP_ENGINE_DIR.......Error   AIBOOTCAMP Engine Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    AIBOOTCAMP_ENGINE_DIR.......Found   ["%AIBOOTCAMP_ENGINE_DIR%"]
    )

    call :RetrieveEngineInfo
    IF "%AIBOOTCAMP_ENGINE_VERSION%" == "" (
       echo    AIBOOTCAMP_ENGINE_VERSION...Error   AIBOOTCAMP Engine Not Found
       set PrerequisitesFound=1
    ) ELSE (
       echo    AIBOOTCAMP_ENGINE_VERSION...Found   ["%AIBOOTCAMP_ENGINE_VERSION%"]
    )

    exit /B %PrerequisitesFound%

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

:RetrieveEngineInfo
    set AIBOOTCAMP_ENGINE_VERSION_FILE=%AIBOOTCAMP_ENGINE_DIR%\Version.txt
    IF EXIST "%AIBOOTCAMP_ENGINE_VERSION_FILE%" (
        FOR /F "usebackq delims== tokens=1,2" %%G IN ("%AIBOOTCAMP_ENGINE_VERSION_FILE%") DO set %%G=%%H
    )
    exit /B 0

:success
    echo.
    echo    Checking Prerequisites...SUCCESS
    goto end

:fail
    echo.
    echo    Checking Prerequisites...FAILED
    goto end

:end