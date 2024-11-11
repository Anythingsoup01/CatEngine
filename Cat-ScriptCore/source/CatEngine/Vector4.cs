using System;

namespace CatEngine
{
	public struct Vector4
	{
		public float X;
		public float Y;
		public float Z;
		public float W;
		public float this[int index]
		{
			get
			{
				switch (index)
				{
					case 0: return X;
					case 1: return Y;
					case 2: return Z;
					case 3: return W;
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
		public Vector4 normalized
		{
			get
			{
				Vector4 vec4 = new Vector4(X, Y, Z, W);
				vec4.Normalize();
				return vec4;
			}
		}

		public float magnitude { get { return (float)Math.Sqrt(X * X + Y * Y + Z * Z); } }
		public Vector4(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		public Vector4(Vector4 other)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
		}
		public Vector4(Vector3 other, float w)
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = w;
		}
		public Vector4(Vector2 other, float z, float w)
		{
			X = other.X;
			Y = other.Y;
			Z = z;
			W = w;
		}
		public Vector4(Vector2 other1, Vector2 other2)
		{
			X = other1.X;
			Y = other1.Y;
			Z = other2.X;
			W = other2.Y;
		}

		public Vector4(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
			W = scalar;
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

		public Vector2 ZW
		{
			get
			{
				return new Vector2(Z, W);
			}
			set
			{
				Z = value.X;
				W = value.Y;
			}
		}

		public static Vector4 operator *(Vector4 vec, float num) => new Vector4(vec.X * num, vec.Y * num, vec.Z * num, vec.W * num);
		public static Vector4 operator *(Vector4 vec1, Vector4 vec2) => new Vector4(vec1.X * vec2.X, vec1.Y * vec2.Y, vec1.Z * vec2.Z, vec1.W * vec2.W);
		public static Vector4 operator +(Vector4 vec1, Vector4 vec2) => new Vector4(vec1.X + vec2.X, vec1.Y + vec2.Y, vec1.Z + vec2.Z, vec1.W + vec2.W);
		public static Vector4 operator /(Vector4 vec, float fl) => new Vector4(vec.X / fl, vec.Y / fl, vec.Z / fl, vec.W / fl);

		static readonly Vector4 zeroVector = new Vector4(0f, 0f, 0f, 0f);
		static readonly Vector4 oneVector = new Vector4(1f, 1f, 1f, 1f);
		static readonly Vector4 upVector = new Vector4(0f, 1f, 0f, 0f);
		static readonly Vector4 downVector = new Vector4(0f, -1f, 0f, 0f);
		static readonly Vector4 leftVector = new Vector4(-1f, 0f, 0f, 0f);
		static readonly Vector4 rightVector = new Vector4(1f, 0f, 0f, 0f);
		static readonly Vector4 forwardVector = new Vector4(0f, 0f, 1f, 0f);

		public static Vector4 zero { get { return zeroVector; } }
		public static Vector4 forward { get { return forwardVector; } }

		public const float cEpsilon = 0.00001F;
	}
}
