using System;
using System.Runtime.CompilerServices;

namespace CatEngine
{
	internal class InternalCalls
	{

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type component);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetPosition(ulong entityID, out Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetPosition(ulong entityID, ref Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keyCode);
	}
}
