using System;
using System.Runtime.CompilerServices;

namespace CatEngine
{

	public class MeownoBehaviour
	{
		protected MeownoBehaviour() { ID = 0; }
		internal MeownoBehaviour(ulong id)
		{ 
			ID = id;
		} 
		public readonly ulong ID;


	}
}

