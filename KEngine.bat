cd %~dp0

xcopy KEngine\Inc\*.h Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Inc\*.inl Engine\Inc\ /d /s /y /exclude:ignore.txt
xcopy KEngine\Bin\*.* Engine\Bin\ /d /s /y /exclude:ignore.txt