#include "inputclass.h"

InputClass::InputClass()
{

}

InputClass::InputClass(const InputClass& other)
{

}

InputClass::~InputClass()
{

}

void InputClass::Initialise()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
	return;
}

void InputClass::KeyDown(unsigned int input)
{
	//if a key is pressed, save the state in the key array
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	//if key is released, clear state in array
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	//return state of key
	return m_keys[key];
}
