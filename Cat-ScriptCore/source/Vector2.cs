namespace CatEngine
{
	public struct Vector2
	{
		public float X, Y;

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

		public static Vector2 Zero => new Vector2(0f);

		public Vector2(float scalar)
		{
			X = scalar;
			Y = scalar;
		}
		public static Vector2 operator *(Vector2 vec, float num) => new Vector2(vec.X * num, vec.Y * num);
		public static Vector2 operator +(Vector2 vec1, Vector2 vec2) => new Vector2(vec1.X + vec2.X, vec1.Y + vec2.Y);
	}
}
