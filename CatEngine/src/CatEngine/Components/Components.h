#pragma once

#include "2D/CircleRenderer.h"
#include "2D/SpriteRenderer.h"

#include "3D/TransformComponent.h"

#include "Physics/BoxCollider2D.h"
#include "Physics/CircleCollider2D.h"
#include "Physics/Rigidbody2D.h"

#include "Renderer/CameraComponent.h"

#include "Required/InformationComponents.h"

#include "Scripting/NativeScriptingComponent.h"


namespace CatEngine
{
	template<typename... Component>
	struct ComponentGroup
	{

	};

	using AllComponents =
		ComponentGroup<TagComponent, LayerComponent, TransformComponent, CameraComponent,
		SpriteRendererComponent, CircleRendererComponent,
		Rigidbody2DComponent, BoxCollider2DComponent, CircleCollider2DComponent,
		NativeScriptComponent>;
}
