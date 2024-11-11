using System;

namespace CatEngine
{
	public abstract class Component : CatEngine.Object
	{

		public Object Object { get; internal set; }
		public Tag tag
		{
			get
			{
				return GetComponent<Tag>();
			}
		}

		public Layer layer
		{
			get
			{
				return GetComponent<Layer>();
			}
		}

		public Transform transform
		{
			get
			{
				return GetComponent<Transform>();
			}
		}

	}
}
