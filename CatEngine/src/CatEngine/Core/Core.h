#pragma once

#ifdef CE_PLATFORM_WINDOWS
	#ifdef CE_USING_DLL
		#ifdef CE_BUILD_DLL
			#define Hazel_API __declspec(dllexport)
			#define CE_API_STATUS "Using CatEngine as a Dynamic Lib"
		#else
			#define CE_API __declspec(dllimport)
			#define CE_API_STATUS "Using CatEngine as a Dynamic Lib"
		#endif // CE_BUILD_DLL

	#else
		#define CE_API
		#define CE_API_STATUS "Using CatEngine as a Static Lib"
	#endif // CE_USING_DLL


#endif // CE_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
