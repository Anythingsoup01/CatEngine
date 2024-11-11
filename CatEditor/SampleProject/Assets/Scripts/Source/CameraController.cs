using System;
using CatEngine;

public class CameraController : MeownoBehaviour
{

	public float m_CameraDistanceFromPlayer = 5.0f;

	private CatEngine.Object m_PlayerControllerObject;
	private PlayerController m_PlayerController;
	private Transform m_Transform;

	void Start()
	{
		Console.WriteLine("Camera Controller on start");

		CatEngine.Object playerObj = FindObjectByName("Player");
		if (playerObj != null)
		{
			Console.WriteLine("Player not null");
			m_PlayerControllerObject = playerObj;
		}
		m_Transform = transform;
		Console.WriteLine("Camera Controller end");

	}

	private void Update(float ts)
	{
		m_Transform.position =  new Vector3(m_PlayerControllerObject.transform.position.XY, m_CameraDistanceFromPlayer);
	}

}

