#pragma once

namespace CatEngine {

	class Time
	{
	public:

		Time(float time = 0.0f)
			: m_Time(time) {}

		float deltaTime() { return m_Time; }

		// Get Delta Time in milliseconds
		float deltaTimeMS() { return m_Time * 1000; }


	private:
		float m_Time;
	};

}