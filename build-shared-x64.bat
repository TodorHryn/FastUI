"%VCPKG_PATH%\vcpkg" install glm --triplet x64-windows
"%VCPKG_PATH%\vcpkg" install glfw3 --triplet x64-windows
"%VCPKG_PATH%\vcpkg" install glad --triplet x64-windows
"%VCPKG_PATH%\vcpkg" install stb --triplet x64-windows
"%VCPKG_PATH%\vcpkg" install freetype --triplet x64-windows
"%VCPKG_PATH%\vcpkg" install icu --triplet x64-windows
cmake -S . -B build-shared -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_TOOLCHAIN_FILE="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows -DBUILD_SHARED_LIBS=ON
cmake --build build-shared