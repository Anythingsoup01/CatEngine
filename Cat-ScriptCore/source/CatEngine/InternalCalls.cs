using System;
using System.Runtime.CompilerServices;

namespace CatEngine
{
	public static class InternalCalls
	{
		#region Object

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Object_HasComponent(ulong instanceID, Type componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static ulong Object_FindObjectByName(string name);

		#endregion

		#region

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static object GetScriptInstance(ulong instanceID);

		#endregion

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keyCode);

		#region Rigidbody2D
		
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyForce(ulong instanceID, ref Vector2 impulse, ref Vector2 point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyForceToCenter(ulong instanceID, ref Vector2 impulse, bool wake);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyLinearImpulse(ulong instanceID, ref Vector2 impulse, ref Vector2 point, bool wake);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Rigidbody2D_ApplyLinearImpulseToCenter(ulong instanceID, ref Vector2 impulse, bool wake);

		#endregion

		#region Transform

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetPosition(ulong instanceID, out Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetPosition(ulong instanceID, ref Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetRotation(ulong instanceID, out Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetRotation(ulong instanceID, ref Vector3 rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_GetScale(ulong instanceID, out Vector3 scale);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Transform_SetScale(ulong instanceID, ref Vector3 scale);
		#endregion
	}
}
