set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${VCPKG_ROOT_DIR}/scripts/toolchains/windows.cmake)
set(VCPKG_BUILD_TYPE release)

set(ENV{CC} cl.exe)
set(ENV{CXX} cl.exe)

# vcpkg's meson cross-detection only flags a build as cross when the CPU
# family differs (or for MinGW/UWP/Android). A same-arch Linux->Windows
# build via msvc-wine is otherwise treated as native, which makes meson
# try to execute the compiled Windows sanity-check .exe directly and fail.
# Force cross mode with an explicit host_machine file. The binaries paths
# are machine-specific (msvc-wine install location), so this is generated
# into a local cache dir rather than committed, from the MSVC_WINE_BIN env
# var set by CMakeUserPresets.json.
if(NOT DEFINED ENV{MSVC_WINE_BIN})
    message(FATAL_ERROR "MSVC_WINE_BIN environment variable is not set; it must point at the msvc-wine bin/x64 directory (see CMakeUserPresets.json).")
endif()
set(Z_MESON_CROSS_FILE "$ENV{HOME}/.cache/msvc-wine-vcpkg/meson-cross-windows-x64.ini")
file(WRITE "${Z_MESON_CROSS_FILE}" "\
[binaries]
c = ['$ENV{MSVC_WINE_BIN}/cl.exe']
cpp = ['$ENV{MSVC_WINE_BIN}/cl.exe']
c_ld = ['$ENV{MSVC_WINE_BIN}/link.exe']
cpp_ld = ['$ENV{MSVC_WINE_BIN}/link.exe']
ar = ['$ENV{MSVC_WINE_BIN}/lib.exe', '/machine:x64', '/nologo']
windres = ['$ENV{MSVC_WINE_BIN}/rc.exe']
mt = ['$ENV{MSVC_WINE_BIN}/mt.exe']

[host_machine]
system = 'windows'
cpu_family = 'x86_64'
cpu = 'x86_64'
endian = 'little'
")
set(VCPKG_MESON_CROSS_FILE "${Z_MESON_CROSS_FILE}")

# tomlplusplus's meson.build runs `cmake --system-information` internally
# (to write a CMake package-version file) when generate_cmake_config is on.
# That ad-hoc probe ignores our cross file and misdetects cl.exe under Wine.
# We don't need the generated CMake config, so just turn it off.
# NOTE: applies triplet-wide; revisit if another meson-based port is added
# that doesn't define this option.
set(VCPKG_MESON_CONFIGURE_OPTIONS -Dgenerate_cmake_config=false)
