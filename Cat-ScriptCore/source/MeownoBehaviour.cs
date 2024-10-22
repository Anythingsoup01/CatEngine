using System.Runtime.CompilerServices;

namespace CatEngine
{

	public class MeownoBehaviour
	{
		protected MeownoBehaviour() { ID = 0; }
		internal MeownoBehaviour(ulong id)
		{
			ID = id;
			sID = id;
		}
		public readonly ulong ID;
		private static ulong sID;

		public static ulong GetID()
		{
			return sID;			
		}
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Entity_GetPosition(ulong entityID, out Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Entity_SetPosition(ulong entityID, ref Vector3 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsKeyDown(KeyCode keyCode);

	}
}

