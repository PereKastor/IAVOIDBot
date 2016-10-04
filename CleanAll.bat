
@echo off
cls

echo You are about to clean up all temporary files in this project... Press enter to continue
pause

cls

echo Starting Project Cleanup

@echo off
del *.job>NUL
del jobfiles7z.txt>NUL

rm -rf build-solution-x64 /Q /S

if exist build-solution-x64 ( 
		echo -- Failed to remove build-solution-x64
		goto fail
	)

	echo -- Cleaned Up build-solution-x64 successfully
	goto end

:success
	echo Project cleaned up successfully
	pause
	goto end

:fail
	echo Failure in cleaning project
	pause
	goto end

:end
