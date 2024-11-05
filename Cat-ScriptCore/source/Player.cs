using System;
using CatEngine;

public class Player : MeownoBehaviour
{
	public float m_Speed = 5f;
	public Vector3 m_Velocity;

	public float m_Vertical;
	public float m_Horizontal;

	private Transform m_Transform;
	private Rigidbody2D m_Rigidbody2D;

	private string m_Tag;

	private void Start()
	{
		m_Transform = GetComponent<Transform>();
		m_Rigidbody2D = GetComponent<Rigidbody2D>();
	}

	private void Update(float ts)
	{

		m_Velocity = Vector3.zero;

		Vector2 vertical;
		Vector2 horizontal;

		if (Input.IsKeyDown(KeyCode.W))
			vertical.X = 1f;
		else
			vertical.X = 0f;
		if (Input.IsKeyDown(KeyCode.S))
			vertical.Y = -1f;
		else
			vertical.Y = 0f;
		if (Input.IsKeyDown(KeyCode.D))
			horizontal.X = 1f;
		else
			horizontal.X = 0f;
		if (Input.IsKeyDown(KeyCode.A))
			horizontal.Y = -1f;
		else
			horizontal.Y = 0f;

		if (Input.IsKeyDown(KeyCode.R))
			m_Transform.Position = Vector3.zero;

		m_Vertical = vertical.X + vertical.Y;
		m_Horizontal = horizontal.X + horizontal.Y;

		m_Velocity = new Vector3(m_Horizontal, m_Vertical, 0f).normalized;

		m_Rigidbody2D.ApplyLinearImpulse(m_Velocity.XY, true);


		//Vector3 pos = m_Transform.Position;
		//pos += m_Velocity * m_Speed * ts;
		//m_Transform.Position = pos;
	}

}

