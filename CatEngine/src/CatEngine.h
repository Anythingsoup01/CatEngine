#pragma once

// For use by CatEngine Applications

#include "CatEngine/Application.h"
#include "CatEngine/Core/Layers/Layer.h"
#include "CatEngine/Core/Log.h"

#include "CatEngine/Core/TimeStep.h"

#include "CatEngine/Core/Input.h"
#include "CatEngine/Core/KeyCodes.h"
#include "CatEngine/Core/MouseButtonCodes.h"

#include "CatEngine/ImGui/ImGuiLayer.h"

// Renderer ------------------------------------------
#include "CatEngine/Core/Renderer/Buffer.h"
#include "CatEngine/Core/Renderer/VertexArray.h"
#include "CatEngine/Core/Renderer/Shader.h"
#include "CatEngine/Core/Renderer/Texture.h"
#include "CatEngine/Core/Renderer/SubTexture2D.h"

#include "CatEngine/Core/Renderer/Renderer.h"
#include "CatEngine/Core/Renderer/Renderer2D.h"
#include "CatEngine/Core/Renderer/RenderCommand.h"

#include "CatEngine/Core/Camera/Camera.h"
#include "CatEngine/Core/Camera/OrthographicCameraController.h"

// Profiling -----------------------------------------
#include "CatEngine/Core/Timer.h"
#include "CatEngine/Debug/Instrumentor.h"
