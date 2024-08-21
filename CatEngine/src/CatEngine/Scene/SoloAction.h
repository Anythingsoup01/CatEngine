#pragma once

#include "Entity.h"

namespace CatEngine
{
	class SoloAction
	{
	public:
		virtual ~SoloAction() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void Start() {}
		virtual void Update(Time time) {}
		virtual void OnDestory() {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}