workspace "CatEngine"
	architecture "x64"

	configurations {
		"Debug",
		"Dist",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "CatEngine/vendors/GLFW/include"
IncludeDir["Glad"] = "CatEngine/vendors/Glad/include"
IncludeDir["ImGui"] = "CatEngine/vendors/ImGui"
IncludeDir["glm"] = "CatEngine/vendors/glm"

include "CatEngine/vendors/GLFW"
include "CatEngine/vendors/Glad"
include "CatEngine/vendors/ImGui"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"CatEngine/src",
		"CatEngine/vendors/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
	}

	links{
		"CatEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"CE_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		optimize "on"

project "CatEngine"
	location "CatEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" ..outputdir.. "/%{prj.name}")
	objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

	pchheader "cepch.h"
	pchsource "CatEngine/src/cepch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendors/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
	}

		defines
		{
			"_CRT_SECURE_NO_WARNINGS",
		}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"CE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
		}

	filter "configurations:Debug"
		defines "CE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CE_DIST"
		runtime "Release"
		optimize "on"
