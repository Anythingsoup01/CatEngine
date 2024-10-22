using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CatEngine
{
	public abstract class Component
	{
		public MeownoBehaviour MeownoBehaviour { get; internal set; }

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
			return InternalCalls.Entity_HasComponent(MeownoBehaviour.ID, componentType);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T();
			return component;
		}
	}
}
