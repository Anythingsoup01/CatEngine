local CatEngineRootDir = '../../../../'

workspace "SampleProjectName"
	architecture "x86_64"
	startproject "AssemblyC#"

	configurations {
		"Debug",
		"Dist",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AssemblyC#"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")


	files {
		"Source/**.cs",
		"Properties/**.cs",
	}

	links
	{
		"Cat-ScriptCore"
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"


	filter "configurations:Release"
		optimize "On"
		symbols "Default"


	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "CatEngine"
	include (CatEngineRootDir .. "Cat-ScriptCore")
group ""
