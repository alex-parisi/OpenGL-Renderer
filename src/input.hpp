///////////////
// input.hpp //
///////////////

#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

class Input
{
	public:
		// Constructor & Destructor
		Input();
		~Input();
		// Get/Set
		int GetKeyState(int);
		void SetKeyState(int key, int state);
		// Add new key
		void AddKey(int key);
		// Check existing key
		bool CheckKey(int key);
		// Reset all keys to not pressed
		void Reset();

	private:
		std::unordered_map<int, int> keyState;
};

class Keyboard : public Input
{
	public:
		// Constructor & Destructor
		Keyboard();
		~Keyboard();
};

class Mouse : public Input
{
	public:
		// Constructor & Destructor
		Mouse();
		~Mouse();
		// Get/Set
		float GetPosX();
		float GetPosY();
		void SetPosX(float newPosX);
		void SetPosY(float newPosY);

	private:
		float posX;
		float posY;
};

struct InputManager
{
	Keyboard m_keyboard;
	Mouse m_mouse;
};