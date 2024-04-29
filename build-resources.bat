xcopy /s "GadgetEngine\Graphics\OpenGL\Shaders" "Build\Resources\" /Y
xcopy /s "GadgetEngine\Graphics\DX12\Shaders" "Build\Resources\" /Y

START /WAIT /B DataBuilder.lnk -f

@echo off
if %errorlevel%==-1 ( echo Data Build was not successful && PAUSE && EXIT /B 2)
@echo on

xcopy /s "Build\Resources\" "Build\Game\Debug\Resources" /Y /d
xcopy /s "Build\Resources\" "Build\Game\Develop\Resources" /Y /d
xcopy /s "Build\Resources\" "Build\Game\Release\Resources" /Y /d

PAUSE