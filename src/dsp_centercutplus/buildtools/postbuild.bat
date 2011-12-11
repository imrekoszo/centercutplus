@echo off

:: 
:: This is a general post build command bat.
:: Put all environment-dependent commands (like copying the compiled dll into the
:: winamp plugins folder) into localpostbuildcommand.bat in this folder.
:: localpostbuildcommand.bat is not version-controlled.
:: 

set postbuildCommand=%~dp0localpostbuildcommand.bat
if exist %postbuildCommand% (
	call %postbuildCommand% %*
) else (
	echo Put local post build commands into %postbuildCommand%.
)
