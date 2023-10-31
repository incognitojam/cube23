$SHADERC_PATH = "$env:VK_SDK_PATH\Bin\glslc.exe"
$SHADERS_DIR = Split-Path -Parent $MyInvocation.MyCommand.Path

Push-Location $SHADERS_DIR

& $SHADERC_PATH shader.vert -o vert.spv
& $SHADERC_PATH shader.frag -o frag.spv

Pop-Location
