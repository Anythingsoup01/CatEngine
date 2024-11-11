#pragma once

#include "CatEngine/Components/Physics/Rigidbody2D.h"

#include <glm/glm.hpp>

#include <box2d/b2_body.h>

namespace CatEngine
{
	class Physics2D
	{
	public:
		static inline void ApplyForce(Entity& entity, glm::vec2& impulse, glm::vec2& point, bool wake)
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyForce(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake);
		}
		static inline void ApplyForceToCenter(Entity& entity, glm::vec2& impulse, bool wake)
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyForceToCenter(b2Vec2(impulse.x, impulse.y), wake);
		}
		static inline void ApplyLinearImpulse(Entity& entity, glm::vec2& impulse, glm::vec2& point, bool wake)
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), wake);
		}
		static inline void ApplyLinearImpulseToCenter(Entity& entity, glm::vec2& impulse, bool wake)
		{
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), wake);
		}
	};
}
