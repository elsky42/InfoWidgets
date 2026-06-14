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
