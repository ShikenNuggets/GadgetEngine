xcopy /s "GadgetEngine\Graphics\OpenGL\Shaders" "Build\Resources\" /Y
xcopy /s "GadgetEngine\Graphics\DX12\Shaders" "Build\Resources\" /Y

START /WAIT /B DataBuilder.lnk

PAUSE