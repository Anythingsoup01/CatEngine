using System;
using System.Runtime.CompilerServices;

namespace CatEngine
{
	public static class InternalCalls
	{

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keyCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetPosition(ulong entityID, out Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetPosition(ulong entityID, ref Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);
	}
}
