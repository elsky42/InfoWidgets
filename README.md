# InfoWidgets

![C++ Standard](https://img.shields.io/badge/standard-c%2B%2B23-blue?logo=cplusplus)
![Platform](https://img.shields.io/badge/platform-windows-blue)
![Release](https://img.shields.io/github/v/release/elsky42/skyrim_infowidgets?logo=hackthebox)

InfoWidgets is a [SKSE](https://skse.silverlock.org/) plugin that adds new HUD elements to Skyrim.

## Prerequisites

- [CMake](https://cmake.org/) 3.21+
- Visual Studio 2022 with the **Desktop development with C++** workload
- [vcpkg](https://vcpkg.io) with the `VCPKG_ROOT` environment variable pointing to its directory

## Building

```bat
git clone --recurse-submodules https://github.com/elsky42/skyrim_infowidgets.git
cd skyrim_infowidgets
cmake --preset release
cmake --build --preset release --parallel
```

The plugin zip will be at `build/InfoWidget-<version>.zip`.

## Building on Linux

For fast local iteration without a Windows machine, the project can be cross-compiled using the real MSVC toolchain running under Wine, via [msvc-wine](https://github.com/mstorsjo/msvc-wine). This should produce the same binary as the Windows build. Keep in mind that the CI builds remain the source of truth for releases.

### Prerequisites

- Wine (e.g. `winehq-staging`)
- Python 3, `msitools`, `winbind`, CMake 3.21+, Ninja
- ~5 GB disk space for the downloaded MSVC toolchain and Windows SDK

### One-time setup

1. Download and install the MSVC toolchain with msvc-wine:

   ```bash
   git clone https://github.com/mstorsjo/msvc-wine.git
   cd msvc-wine
   python3 vsdownload.py --accept-license --dest ~/msvc-wine/msvc
   ./install.sh ~/msvc-wine/msvc
   ```

2. Clone and bootstrap vcpkg, if you don't already have one set up:

   ```bash
   git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
   ~/vcpkg/bootstrap-vcpkg.sh -disableMetrics
   ```

3. Create a `CMakeUserPresets.json` in the repo root (gitignored, machine-specific) with a preset pointing at your install locations:

   ```json
   {
     "version": 3,
     "configurePresets": [
       {
         "name": "linux-wine",
         "inherits": "base",
         "generator": "Ninja",
         "binaryDir": "${sourceDir}/build-linux-wine",
         "environment": {
           "VCPKG_ROOT": "/home/you/vcpkg",
           "MSVC_WINE_BIN": "/home/you/msvc-wine/msvc/bin/x64",
           "PATH": "$env{MSVC_WINE_BIN}:$penv{PATH}"
         },
         "cacheVariables": {
           "CMAKE_SYSTEM_NAME": "Windows",
           "CMAKE_C_COMPILER": "cl",
           "CMAKE_CXX_COMPILER": "cl",
           "VCPKG_OVERLAY_TRIPLETS": "${sourceDir}/cmake/triplets-wine",
           "VCPKG_OVERLAY_PORTS": "${sourceDir}/cmake/ports-wine"
         }
       }
     ],
     "buildPresets": [
       { "name": "linux-wine", "configurePreset": "linux-wine" }
     ]
   }
   ```

   Adjust `VCPKG_ROOT` and `MSVC_WINE_BIN` to match where you installed them in steps 1-2.

### Build

```bash
cmake --preset linux-wine
cmake --build --preset linux-wine --parallel
```

The plugin zip will be at `build-linux-wine/InfoWidget-<version>.zip`.
