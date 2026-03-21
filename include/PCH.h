#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <toml++/toml.hpp>

#pragma warning(push)
#pragma warning(disable: 4099)  // struct/class mismatch in third-party header
#pragma warning(disable: 5054)  // enum operator '|' deprecated in third-party header
#include "SKSEMenuFramework.h"
#pragma warning(pop)
#include "IconsFontAwesome6.h"

#include "Version.h"

