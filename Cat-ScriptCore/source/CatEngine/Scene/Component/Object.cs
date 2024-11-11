using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CatEngine
{
	public class Object
	{
		protected Object() { m_InstanceID = 0; }

		internal Object(ulong id)
		{
			m_InstanceID = id;
		}

		public readonly ulong m_InstanceID;

		public Transform transform
		{
			get
			{
				return GetComponent<Transform>();
			}
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type componentType = typeof(T);
			return InternalCalls.Object_HasComponent(m_InstanceID, componentType);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Object = this };
			return component;
		}

		public Object FindObjectByName(string name)
		{
			ulong objectID = InternalCalls.Object_FindObjectByName(name);
			if (objectID == 0)
				return null;
			return new Object(objectID) { };
		}

		public T As<T>() where T : Object, new()
		{
			object instance = InternalCalls.GetScriptInstance(m_InstanceID);
			return instance as T;
		}
	}
}
