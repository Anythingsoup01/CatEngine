using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CatEngine
{
	public class Transform : Component
	{
		public Vector3 position
		{
			get
			{
				InternalCalls.Transform_GetPosition(Object.m_InstanceID, out Vector3 position);
				return position;
			}
			set
			{
				InternalCalls.Transform_SetPosition(Object.m_InstanceID, ref value);
			}
		}
		public Vector3 rotation
		{
			get
			{
				InternalCalls.Transform_GetRotation(Object.m_InstanceID, out Vector3 rotation);
				return rotation;
			}
			set
			{
				InternalCalls.Transform_SetRotation(Object.m_InstanceID, ref value);
			}
		}

		public Vector3 scale
		{
			get
			{
				InternalCalls.Transform_GetScale(Object.m_InstanceID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.Transform_SetScale(Object.m_InstanceID, ref value);
			}
		}

	}
}
