#pragma once

namespace CatEngine
{
	class SoloAction;

	struct NativeScriptComponent
	{
		SoloAction* Instance = nullptr;


		SoloAction* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<SoloAction*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}