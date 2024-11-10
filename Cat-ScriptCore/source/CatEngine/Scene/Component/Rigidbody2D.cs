
namespace CatEngine
{
	public class Rigidbody2D : Component
	{
		public void ApplyForce(Vector2 impulse, Vector2 worldPoint, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyForce(Object.m_InstanceID, ref impulse, ref worldPoint, wake);
		}
		public void ApplyForce(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyForceToCenter(Object.m_InstanceID, ref impulse, wake);
		}
		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPoint, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyLinearImpulse(Object.m_InstanceID, ref impulse, ref worldPoint, wake);
		}
		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2D_ApplyLinearImpulseToCenter(Object.m_InstanceID, ref impulse, wake);
		}
	}
}
