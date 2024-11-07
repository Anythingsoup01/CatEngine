CatEngineRootDir = '../'

project "Cat-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir (CatEngineRootDir .. "/CatEditor/Resources/Scripts")
	objdir (CatEngineRootDir .. "/CatEditor/Resources/Intermediates")


	files {
		"Source/**.cs",
		"Properties/**.cs",
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

