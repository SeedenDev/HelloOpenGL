@echo off

pushd ..
rem Can be vs2022/vs2026
cmake --preset "windows-x64-msvc-vs2022"
popd
pause