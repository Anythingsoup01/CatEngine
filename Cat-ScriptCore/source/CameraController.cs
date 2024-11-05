using System;
using CatEngine;

public class CameraController : MeownoBehaviour
{
	public float m_Speed = 5f;
	public Vector3 m_Velocity;

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
			position = Vector3.zero;

		float m_Vertical = vertical.X + vertical.Y;
		float  m_Horizontal = horizontal.X + horizontal.Y;

		m_Velocity = new Vector3(m_Horizontal, m_Vertical, 0f).normalized;

		Vector3 pos = position;
		pos += m_Velocity * m_Speed * ts;
		position = pos;
	}

}

