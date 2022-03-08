Remove-Item -Path build -Force -Recurse
Start-Sleep -s 1
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build -j