using System;
using System.Runtime.CompilerServices;
using CatEngine;

public class PlayerController : MeownoBehaviour
{

	public float m_Speed = 1f;
	public Vector2 m_Velocity;

	private float m_Vertical;
	private float m_Horizontal;

//	public CameraController m_CameraController;

	private Transform m_Transform;
	private Rigidbody2D m_Rigidbody2D;

	private void Start()
	{
		m_Transform = GetComponent<Transform>();
		m_Rigidbody2D = GetComponent<Rigidbody2D>();

		Console.WriteLine("Player Controller - Start");
	}

	private void Update(float ts)
	{


		m_Velocity = Vector2.zero;

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


		m_Velocity = new Vector2(m_Horizontal, m_Vertical).normalized * m_Speed * ts;

		m_Rigidbody2D.ApplyForce(m_Velocity, true);


	}


}

