#pragma once

#include "CatEngine/Core/PlatformDetection.h"

#ifdef CE_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "CatEngine/Core/Core.h"
#include "CatEngine/Core/Log.h"
#include "CatEngine/CatDebug/Instrumentor.h"

#ifdef CE_PLATFORM_WINDOWS
#include <Windows.h>
#endif
