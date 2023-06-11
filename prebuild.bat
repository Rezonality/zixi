@echo off

echo %Time%

if not exist "vcpkg\vcpkg.exe" (
    cd vcpkg
    call bootstrap-vcpkg.bat -disableMetrics
    cd %~dp0
)

cd vcpkg
echo Installing Libraries
vcpkg install date tomlplusplus cppcodec imgui concurrentqueue clipp glm tinydir fmt catch2 --triplet x64-windows-static-md --recurse
cd %~dp0
echo %Time%

