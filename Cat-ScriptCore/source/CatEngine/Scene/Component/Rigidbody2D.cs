
namespace CatEngine
{
	public class Rigidbody2D : Component
	{
		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPoint, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyLinearImpulse(Object.ID, ref impulse, ref worldPoint, wake);
		}
		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyLinearImpulseToCenter(Object.ID, ref impulse, wake);
		}
	}
}
