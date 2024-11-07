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
				InternalCalls.Transform_GetPosition(Object.ID, out Vector3 position);
				return position;
			}
			set
			{
				InternalCalls.Transform_SetPosition(Object.ID, ref value);
			}
		}
		public Vector3 Rotation
		{
			get
			{
				InternalCalls.Transform_GetRotation(Object.ID, out Vector3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.Transform_SetRotation(Object.ID, ref value);
			}
		}

		public Vector3 Scale
		{
			get
			{
				InternalCalls.Transform_GetScale(Object.ID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.Transform_SetScale(Object.ID, ref value);
			}
		}

	}
}
