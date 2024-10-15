project "CatEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.EnTT}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.yaml}",
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	links{
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",

		"%{Library.mono}",
	}

	filter "files:vendors/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		links
		{
			"%{Library.WinSock}",
			"%{Library.Winmm}",
			"%{Library.Version}",
			"%{Library.Bcrypt}",
		}

		defines{
			"GLFW_INCLUDE_NONE",
		}


	filter "configurations:Debug"
		defines "CE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "CE_RELEASE"
		runtime "Release"
		optimize "on"
		
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "CE_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
