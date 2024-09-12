#pragma once

#include "CatEngine/Core/Core.h"
#include "CatEngine/Core/Log.h"

#ifdef CE_ENABLE_ASSERTS
// Currently accepts at least the condition and one additional parameter (the message) being optional
#define CE_ASSERT(x) if (!(x)) __debugbreak;
#define CE_API_ASSERT(x, ...) if (!(x)) { CatEngine::Log::GetAPILogger()->critical(__VA_ARGS__); __debugbreak; } 
#else
#define CE_ASSERT(...)
#define CE_API_ASSERT(...)
#endif
