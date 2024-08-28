project "CatEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" ..outputdir.. "/%{prj.name}")

	pchheader "cepch.h"
	pchsource "src/cepch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendors/glm/glm/**.hpp",
		"vendors/glm/glm/**.ini",
		"vendors/stb/**.cpp",
		"vendors/stb/**.h",
		"vendors/ImGuizmo/ImGuizmo.cpp",
		"vendors/ImGuizmo/ImGuizmo.h",
	}

	includedirs{
		"src",
		"vendors/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "files:vendors/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines{
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
