using System;
using System.Runtime.CompilerServices;

namespace CatEngine
{

	public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static void NativeLogString(string text, int paramenter);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static void NativeLogVector3(ref Vector3 paramenter);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static void NativeLogVector2(ref Vector2 paramenter);
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		public extern static Vector3 AddVector3(ref Vector3 value1, ref Vector3 value2);
	}

	public struct Vector3
	{
		public float X, Y, Z;

		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
		public Vector3(Vector3 other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
		}
	}
	public struct Vector2
	{
		public float X, Y;

		public Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}
	}

	public class Entity
	{

		Vector3 position = new Vector3(0.64f, 0.23f, 0.32f);

		void Update()
		{

		}
		void Start()
		{
			LogVector3(ref position);
			Vector3 newPos = new Vector3(1.1f, 1.6f, 1.7f);
			LogVector3(ref newPos);

			LogVector3(new Vector3(InternalCalls.AddVector3(ref position,ref newPos)));
		}
		void LogVector3(ref Vector3 vector)
		{
			InternalCalls.NativeLogVector3(ref vector);
		}
		void LogVector3(Vector3 vector)
		{
			InternalCalls.NativeLogVector3(ref vector);
		}
	}
}
