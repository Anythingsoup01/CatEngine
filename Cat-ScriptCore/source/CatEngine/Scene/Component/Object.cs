using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CatEngine
{
	public class Object
	{
		protected Object() { ID = 0; }

		internal Object(ulong id)
		{
			ID = id;
		}

		public readonly ulong ID;

		
	}
}
