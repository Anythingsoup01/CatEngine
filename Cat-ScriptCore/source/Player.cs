using System;
using CatEngine;

public class Player : MeownoBehaviour
{
	public float m_Speed = 5f;
	public Vector3 m_Velocity;

	private void Start()
	{
		Console.WriteLine("Hello World!");
	}

	private void Update(float ts)
	{
		// Basic movement
		//Vector3 position = Position;
		//position.X += m_Speed * ts;
		//Position = position;

		// Intermediate movement
		m_Velocity = Vector3.Zero;

		if (Input.IsKeyDown(KeyCode.W))
			m_Velocity.Y = 1f;
		if (Input.IsKeyDown(KeyCode.S))
			m_Velocity.Y = -1f;
		if (Input.IsKeyDown(KeyCode.D))
			m_Velocity.X = 1f;
		if (Input.IsKeyDown(KeyCode.A))
			m_Velocity.X = -1f;

		Vector3 position = Transform.Position;

		position += m_Velocity * m_Speed * ts;

		
		
		Transform.Position = position;
	}

}

