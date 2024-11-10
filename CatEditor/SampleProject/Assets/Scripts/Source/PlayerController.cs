using System;
using System.Runtime.CompilerServices;
using CatEngine;

public class PlayerController : MeownoBehaviour
{

	public float m_Speed = 1f;
	public Vector2 m_Velocity;

	public CameraController m_CameraController;

	private Transform m_Transform;
	private Rigidbody2D m_Rigidbody2D;

	private void Start()
	{
		Console.WriteLine("Player Controller - Start");
		m_Transform = transform;
		m_Rigidbody2D = GetComponent<Rigidbody2D>();
		CatEngine.Object camera = FindObjectByName("Camera");
		if (camera != null)
		{
			Console.WriteLine("Camera Controller not null!");
			m_CameraController = camera.As<CameraController>(); 
		}
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

		if (Input.IsKeyDown(KeyCode.Q))
			m_CameraController.m_CameraDistanceFromPlayer -= 0.5f * ts;
		if (Input.IsKeyDown(KeyCode.E))
			m_CameraController.m_CameraDistanceFromPlayer += 0.5f * ts;


		float m_Vertical = vertical.X + vertical.Y;
		float m_Horizontal = horizontal.X + horizontal.Y;


		m_Velocity = new Vector2(m_Horizontal, m_Vertical).normalized * m_Speed * ts;

		m_Rigidbody2D.ApplyLinearImpulse(m_Velocity, true);

	}


}

