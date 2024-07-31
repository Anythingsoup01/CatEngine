#pragma once

// For use by CatEngine Applications

#include "CatEngine/Application.h"
#include "CatEngine/Core/Layers/Layer.h"
#include "CatEngine/Core/Logging/Log.h"

#include "CatEngine/Core/TimeStep.h"

#include "CatEngine/Core/Input/Input.h"
#include "CatEngine/Core/Input/KeyCodes.h"
#include "CatEngine/Core/Input/MouseButtonCodes.h"

#include "CatEngine/ImGui/ImGuiLayer.h"

// Renderer ------------------------------------------
#include "CatEngine/Core/Buffers/Buffer.h"
#include "CatEngine/Core/Buffers/VertexArray.h"
#include "CatEngine/Core/Shader/Shader.h"
#include "CatEngine/Core/Shader/Texture.h"

#include "CatEngine/Core/Renderer/Renderer.h"
#include "CatEngine/Core/Renderer/Renderer2D.h"
#include "CatEngine/Core/Renderer/RenderCommand.h"

#include "CatEngine/Core/Camera/Camera.h"
#include "CatEngine/Core/Camera/OrthographicCameraController.h"

// Profiling -----------------------------------------
#include "CatEngine/Core/Timer.h"
