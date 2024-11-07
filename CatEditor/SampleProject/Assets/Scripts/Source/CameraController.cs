using System;
using CatEngine;

public class FlyingCameraController : MeownoBehaviour
{
	public float m_MoveSpeed = 1f;
	public float m_RotateSpeed = 1f;

	public Vector3 m_Velocity;

	private Transform m_Transform;

	void Start()
	{
		m_Transform = GetComponent<Transform>();
	}

	private void Update(float ts)
	{


	}

}

