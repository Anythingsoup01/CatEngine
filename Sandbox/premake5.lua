project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/CatEngine/vendors/spdlog/include",
		"%{wks.location}/CatEngine/src",
		"%{wks.location}/CatEngine/vendors",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.EnTT}"
	}

	links
	{
		"CatEngine"
	}

	filter "system:windows"
		systemversion "latest"

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