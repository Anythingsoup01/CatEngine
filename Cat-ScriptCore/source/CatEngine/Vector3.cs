using System;

namespace CatEngine
{
	public struct Vector3
	{
		public float X;
		public float Y;
		public float Z;
		public float this[int index]
		{
			get
			{
				switch (index)
				{
					case 0: return X;
					case 1: return Y;
					case 2: return Z;
					default:
						throw new Exception("Invalid Vector3 index!");
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
		public Vector3 normalized
		{
			get
			{
				Vector3 vec3 = new Vector3(X, Y, Z);
				vec3.Normalize();
				return vec3;
			}
		}

		public float magnitude { get { return (float)Math.Sqrt(X * X + Y * Y + Z * Z); } }
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
		public Vector3(Vector2 other, float z)
		{
			X = other.X;
			Y = other.Y;
			Z = z;
		}

		public Vector3(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
		}

		public Vector2 XY
		{
			get
			{
				return new Vector2(X, Y);
			}
			set
			{
				X = value.X;
				Y = value.Y;
			}
		}

		public static Vector3 operator *(Vector3 vec, float num) => new Vector3(vec.X * num, vec.Y * num, vec.Z * num);
		public static Vector3 operator +(Vector3 vec1, Vector3 vec2) => new Vector3(vec1.X + vec2.X, vec1.Y + vec2.Y, vec1.Z + vec2.Z);
		public static Vector3 operator/(Vector3 vec, float fl) => new Vector3(vec.X / fl, vec.Y / fl, vec.Z / fl);

		static readonly Vector3 zeroVector = new Vector3(0f, 0f, 0f);
		static readonly Vector3 oneVector = new Vector3(1f, 1f, 1f);
		static readonly Vector3 upVector = new Vector3(0f, 1f, 0f);
		static readonly Vector3 downVector = new Vector3(0f, -1f, 0f);
		static readonly Vector3 leftVector = new Vector3(-1f, 0f, 0f);
		static readonly Vector3 rightVector = new Vector3(1f, 0f, 0f);
		static readonly Vector3 forwardVector = new Vector3(0f, 0f, 1f);

		public static Vector3 zero { get { return zeroVector; } }

		public const float cEpsilon = 0.00001F;
	}
}
