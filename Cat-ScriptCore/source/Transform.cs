namespace CatEngine
{
	public struct Transform
	{
		public static Vector3 Position
		{
			get
			{
				MeownoBehaviour.Entity_GetPosition(MeownoBehaviour.GetID(), out Vector3 position);
				return position;
			}
			set
			{
				MeownoBehaviour.Entity_SetPosition(MeownoBehaviour.GetID(), ref value);
			}

		}
	}
}
