#pragma once



namespace CatEngine
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h 
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2,
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}


#define CE_MOUSE_BUTTON_1         ::CatEngine::Mouse::Button0
#define CE_MOUSE_BUTTON_2         ::CatEngine::Mouse::Button1
#define CE_MOUSE_BUTTON_3         ::CatEngine::Mouse::Button2
#define CE_MOUSE_BUTTON_4         ::CatEngine::Mouse::Button3
#define CE_MOUSE_BUTTON_5         ::CatEngine::Mouse::Button4
#define CE_MOUSE_BUTTON_6         ::CatEngine::Mouse::Button5
#define CE_MOUSE_BUTTON_7         ::CatEngine::Mouse::Button6
#define CE_MOUSE_BUTTON_8         ::CatEngine::Mouse::Button7
#define CE_MOUSE_BUTTON_LAST      ::CatEngine::Mouse::ButtonLast
#define CE_MOUSE_BUTTON_LEFT      ::CatEngine::Mouse::ButtonLeft
#define CE_MOUSE_BUTTON_RIGHT     ::CatEngine::Mouse::ButtonRight
#define CE_MOUSE_BUTTON_MIDDLE    ::CatEngine::Mouse::ButtonMiddle