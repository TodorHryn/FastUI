"%VCPKG_PATH%\vcpkg" install glm --triplet x64-windows-static
"%VCPKG_PATH%\vcpkg" install glfw3 --triplet x64-windows-static
"%VCPKG_PATH%\vcpkg" install glad --triplet x64-windows-static
"%VCPKG_PATH%\vcpkg" install stb --triplet x64-windows-static
"%VCPKG_PATH%\vcpkg" install freetype --triplet x64-windows-static
"%VCPKG_PATH%\vcpkg" install icu --triplet x64-windows-static
cmake -S . -B build-static -DCMAKE_TOOLCHAIN_FILE="%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static -DBUILD_SHARED_LIBS=OFF
cmake --build build-static