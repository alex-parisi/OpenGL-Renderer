///////////////
// input.cpp //
///////////////

#include "input.hpp"

Input::Input()
{

}

Input::~Input()
{

}

int Input::GetKeyState(int key)
{
	if (CheckKey(key))
		return keyState[key];
	return -1;
}

void Input::SetKeyState(int key, int state)
{
	if (CheckKey(key))
		keyState[key] = state;
}

void Input::AddKey(int key)
{
	if (!CheckKey(key))
		keyState[key] = GLFW_RELEASE;
}

bool Input::CheckKey(int key)
{
	if (keyState.count(key) == 0)
		return false;
	return true;
}

void Input::Reset()
{
	for (auto const& x : keyState)
	{
		keyState[x.first] = GLFW_RELEASE;
	}
}

Keyboard::Keyboard()
{
	// Initialize the keyboard with the keys we'd like to manage:
	AddKey(GLFW_KEY_Q);
	AddKey(GLFW_KEY_W);
	AddKey(GLFW_KEY_E);
	AddKey(GLFW_KEY_A);
	AddKey(GLFW_KEY_S);
	AddKey(GLFW_KEY_D);
	AddKey(GLFW_KEY_Z);
	AddKey(GLFW_KEY_X);

	AddKey(GLFW_KEY_UP);
	AddKey(GLFW_KEY_LEFT);
	AddKey(GLFW_KEY_DOWN);
	AddKey(GLFW_KEY_RIGHT);

	AddKey(GLFW_KEY_SPACE);

	AddKey(GLFW_KEY_1);
	AddKey(GLFW_KEY_2);
}

Keyboard::~Keyboard()
{

}

Mouse::Mouse()
{
	posX = 0.0f;
	posY = 0.0f;
	// Initialize the mouse with the buttons we'd like to manage:
	AddKey(GLFW_MOUSE_BUTTON_LEFT);
	AddKey(GLFW_MOUSE_BUTTON_RIGHT);
	AddKey(GLFW_MOUSE_BUTTON_MIDDLE);
}

Mouse::~Mouse()
{

}

float Mouse::GetPosX()
{
	return posX;
}

float Mouse::GetPosY()
{
	return posY;
}

void Mouse::SetPosX(float newPosX)
{
	posX = newPosX;
}

void Mouse::SetPosY(float newPosY)
{
	posY = newPosY;
}