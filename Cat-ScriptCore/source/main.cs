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

		void Start()
		{
			PrintMessage();
		}

		private void PrintMessage()
		{
			InternalCalls.NativeLogString("You suck!!", 1);
		}

	}
}
