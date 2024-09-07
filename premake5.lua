include "./vendor/bin/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "CatEngine"
	architecture "x86_64"
	startproject "CatEditor"

	configurations {
		"Debug",
		"Dist",
		"Release"
	}
	
	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


group "Dependencies"
	include "CatEngine/vendors/Box2D"
	include "CatEngine/vendors/GLFW"
	include "CatEngine/vendors/Glad"
	include "CatEngine/vendors/ImGui"
	include "CatEngine/vendors/yaml-cpp"
group ""

group "Tools"
	include "CatEditor"
group ""

group "Core"
	include "CatEngine"
group ""

group "Misc"
	include "Sandbox"
group ""