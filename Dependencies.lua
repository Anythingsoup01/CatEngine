
-- CatEngine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/CatEngine/vendors/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/CatEngine/vendors/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/CatEngine/vendors/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/CatEngine/vendors/ImGui"
IncludeDir["glm"] = "%{wks.location}/CatEngine/vendors/glm"
IncludeDir["stb"] = "%{wks.location}/CatEngine/vendors/stb"
IncludeDir["EnTT"] = "%{wks.location}/CatEngine/vendors/EnTT/include"
IncludeDir["yaml"] = "%{wks.location}/CatEngine/vendors/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/CatEngine/vendors/ImGuizmo"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"