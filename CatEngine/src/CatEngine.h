#pragma once

// For use by CatEngine Applications

#include "CatEngine/Application.h"
#include "CatEngine/Layers/Layer.h"
#include "CatEngine/Core/Log.h"

#include "CatEngine/Core/TimeStep.h"

#include "CatEngine/Core/Input.h"
#include "CatEngine/Core/KeyCodes.h"
#include "CatEngine/Core/MouseButtonCodes.h"

#include "CatEngine/ImGui/ImGuiLayer.h"

// Renderer ------------------------------------------
#include "CatEngine/Renderer/Buffer.h"
#include "CatEngine/Renderer/Framebuffer.h"
#include "CatEngine/Renderer/VertexArray.h"
#include "CatEngine/Renderer/Shader.h"
#include "CatEngine/Renderer/Texture.h"
#include "CatEngine/Renderer/SubTexture2D.h"

#include "CatEngine/Renderer/Renderer.h"
#include "CatEngine/Renderer/Renderer2D.h"
#include "CatEngine/Renderer/RenderCommand.h"

#include "CatEngine/Camera/Camera.h"
#include "CatEngine/Camera/EditorCamera.h"
#include "CatEngine/Camera/OrthographicCameraController.h"

// Scene ---------------------------------------------
#include "CatEngine/Scene/Scene.h"
#include "CatEngine/Scene/Entity.h"
#include "CatEngine/Scene/SoloAction.h"
#include "CatEngine/Scene/SceneSerializer.h"

// Runtime -------------------------------------------

#include "CatEngine/Components/Components.h"


// Profiling -----------------------------------------
#include "CatEngine/Core/Timer.h"
#include "CatEngine/Debug/Instrumentor.h"
