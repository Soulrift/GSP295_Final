//=============================================================================
// DirectInput.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Wraps initialization of immediate mode Direct Input, and provides 
// information for querying the state of the keyboard and mouse.
//=============================================================================

#ifndef DIRECT_INPUT_H
#define DIRECT_INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum KEYSTATE 
{
	KEY_NOCHANGE,
	KEY_PRESSED,
	KEY_HELD,
	KEY_RELEASED
};

class DirectInput {
public:
	DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();
	bool keyDown(char key);
	bool mouseButtonDown(int button);

	KEYSTATE keyState(int key);
	bool keypressed(int key);
	bool keyheld(int Key);
	bool keyreleased(int Key);

	float mouseDX();
	float mouseDY();
	float mouseDZ();
	
	float mouseX();
	float mouseY();

private:
	//Make private to prevent copying of members of this class.
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);


		
	IDirectInput8*       mDInput;

	IDirectInputDevice8* mKeyboard;
	char				m_KeyBuffer[256]; //Buffer for keyboard
	char				m_LastKeyBuffer[256]; //Buffer for debounce
	char				m_CurrKeyBuffer[256];

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;

	POINT				 mMousePos; //mouse position
};
extern DirectInput* gDInput;

#endif