cmake -G "Visual Studio 12" -D CMAKE_BUILD_TYPE:String=Release .
msbuild Trillek_Client.vcxproj /p:Configuration=Release /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
cd tests
cmake -G "Visual Studio 12" -D CMAKE_BUILD_TYPE:String=Release .
msbuild TCCTests.vcxproj /p:Configuration=Release /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"

