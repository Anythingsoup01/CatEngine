using System;

namespace CatEngine
{
	public abstract class Component : CatEngine.Object
	{

		public Object Object { get; internal set; }

		public Component gameObject
		{
			get
			{
				return this;
			}
		}

		public string tag
		{
			get
			{
				return gameObject.tag;
			}
			set
			{
				gameObject.tag = value;
			}
		}

		public Layer layer
		{
			get
			{
				return gameObject.layer;
			}
		}

		public Vector3 position
		{
			get
			{
				InternalCalls.Transform_GetPosition(ID, out Vector3 result);
				return result;
			}
			set
			{
				InternalCalls.Transform_SetPosition(ID, ref value);
			}
		}

		public bool HasComponent<T>() where T : Component, new()
		{
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Object = this };
			return component;
		}

	}
}
