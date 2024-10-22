using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CatEngine
{
	public class Transform : Component
	{
		public Vector3 Position
		{
			get
			{
				InternalCalls.Transform_GetPosition(MeownoBehaviour.ID, out Vector3 position);
				return position;
			}
			set
			{
				InternalCalls.Transform_SetPosition(MeownoBehaviour.ID, ref value);
			}
		}


	}
}
