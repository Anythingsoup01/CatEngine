
-- CatEngine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/CatEngine/vendors/Box2D/include"
IncludeDir["EnTT"] = "%{wks.location}/CatEngine/vendors/EnTT/include"
IncludeDir["FileWatch"] = "%{wks.location}/CatEngine/vendors/FileWatch"
IncludeDir["GLFW"] = "%{wks.location}/CatEngine/vendors/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/CatEngine/vendors/Glad/include"
IncludeDir["glm"] = "%{wks.location}/CatEngine/vendors/glm"
IncludeDir["ImGui"] = "%{wks.location}/CatEngine/vendors/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/CatEngine/vendors/ImGuizmo"
IncludeDir["mono"] =  "%{wks.location}/CatEngine/vendors/mono/include"
IncludeDir["stb"] = "%{wks.location}/CatEngine/vendors/stb"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["yaml"] = "%{wks.location}/CatEngine/vendors/yaml-cpp/include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["Mono"] = "%{wks.location}/CatEngine/vendors/mono/lib/%{cfg.buildcfg}"

Library = {}
Library["mono"] = "%{LibraryDir.Mono}/libmono-static-sgen.lib"

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

-- Windows

Library["WinSock"] = "Ws2_32.lib"
Library["Winmm"] = "Winmm.lib"
Library["Version"] = "Version.lib"
Library["Bcrypt"] = "Bcrypt.lib"
