namespace CatEngine
{
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

		public static Vector3 Zero => new Vector3(0f);

		public Vector3(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
		}

		public static Vector3 operator *(Vector3 vec, float num) => new Vector3(vec.X * num, vec.Y * num, vec.Z * num);
		public static Vector3 operator +(Vector3 vec1, Vector3 vec2) => new Vector3(vec1.X + vec2.X, vec1.Y + vec2.Y, vec1.Z + vec2.Z);
	}
}
