#pragma once

namespace CatEngine {

	class Time
	{
	public:

		Time(float time = 0.0f)
			: m_Time(time) {}

		inline float deltaTime() { return m_Time; }

		// Get Delta Time in milliseconds
		inline float deltaTimeMS() { return m_Time * 1000; }

		static float GetTime();


	private:
		float m_Time;
	};

}