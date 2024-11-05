using System;

namespace CatEngine
{
	public struct Vector2
	{
		public float X;
		public float Y;

		public float this[int index]
		{
			get
			{
				switch (index)
				{
					case 0: return X;
					case 1: return Y;
					default:
						throw new Exception("Invalid Vector2 index!");
				}
			}
		}
		public void Normalize()
		{
			float mag = magnitude;
			if (mag > cEpsilon)
				this = this / mag;
			else
				this = zero;

		}
		public Vector2 normalized
		{
			get
			{
				Vector2 vec2 = new Vector2(X, Y);
				vec2.Normalize();
				return vec2;
			}
		}

		public float magnitude { get { return (float)Math.Sqrt(X * X + Y * Y); } }




		public Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}
		public Vector2(Vector2 other)
		{
			X = other.X;
			Y = other.Y;
		}
		public Vector2(float scalar)
		{
			X = scalar;
			Y = scalar;
		}

		public static Vector2 operator*(Vector2 vec, float num) => new Vector2(vec.X * num, vec.Y * num);
		public static Vector2 operator+(Vector2 vec1, Vector2 vec2) => new Vector2(vec1.X + vec2.X, vec1.Y + vec2.Y);
		public static Vector2 operator/(Vector2 vec, float fl) => new Vector2(vec.X / fl, vec.Y / fl);

		static readonly Vector2 zeroVector = new Vector2(0f, 0f);
		static readonly Vector2 oneVector = new Vector2(1f, 1f);
		static readonly Vector2 upVector = new Vector2(0f, 1f);
		static readonly Vector2 downVector = new Vector2(0f, -1f);
		static readonly Vector2 leftVector = new Vector2(-1f, 0f);
		static readonly Vector2 rightVector = new Vector2(1f, 0f);

		public static Vector2 zero { get { return zeroVector; } }
		public static Vector2 up { get { return upVector; } }

		public const float cEpsilon = 0.00001F;


	}
}
