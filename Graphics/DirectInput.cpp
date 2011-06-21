//=============================================================================
// DirectInput.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

/*
This code was expanded upon by Cameron Ferguson and Heath Wickmen
*/

#include "d3dUtil.h"
#include "DirectInput.h"
#include "d3dApp.h"
//#include "GameSettings.h"

DirectInput* gDInput = 0;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags) {

	ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));
	ZeroMemory(m_LastKeyBuffer, sizeof(m_LastKeyBuffer));
	ZeroMemory(m_CurrKeyBuffer, sizeof(m_CurrKeyBuffer));

	DirectInput8Create(gd3dApp->getAppInst(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, 0);

	mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0);
	mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	mKeyboard->SetCooperativeLevel(gd3dApp->getMainWnd(), keyboardCoopFlags);
	mKeyboard->Acquire();

	mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0);
	mMouse->SetDataFormat(&c_dfDIMouse2);
	mMouse->SetCooperativeLevel(gd3dApp->getMainWnd(), mouseCoopFlags);
	mMouse->Acquire();

}


DirectInput::~DirectInput() {

	ReleaseCOM(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	ReleaseCOM(mKeyboard);
	ReleaseCOM(mMouse);
}

void DirectInput::poll() {
	//Poll keyboard.
	HRESULT hr = mKeyboard->GetDeviceState(sizeof(m_KeyBuffer), (void**)&m_KeyBuffer); 
	if( FAILED(hr) )
	{
		//Keyboard lost, zero out keyboard data structure.
		ZeroMemory(m_KeyBuffer, sizeof(m_KeyBuffer));

		 //Try to acquire for next time we poll.
		hr = mKeyboard->Acquire();
	}

	//Poll mouse.
	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&mMouseState); 
	if( FAILED(hr) )
	{
		//Mouse lost, zero out mouse data structure.
		ZeroMemory(&mMouseState, sizeof(mMouseState));

		//Try to acquire for next time we poll.
		hr = mMouse->Acquire();
	}

	//get the actual coordinates of the mouse on the screen
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(gd3dApp->getMainWnd(), &mouse);

	mMousePos.x = mouse.x;
	mMousePos.y = mouse.y;

}

bool DirectInput::keyDown(char key) {

	return (m_KeyBuffer[key] & 0x80) != 0;
}

KEYSTATE DirectInput::keyState(int key)
{
	KEYSTATE returnValue;

	m_CurrKeyBuffer[key] = ((m_KeyBuffer[key] & 0x80) != 0);

	if((m_CurrKeyBuffer[key] != m_LastKeyBuffer[key]) && (m_LastKeyBuffer[key] == 0))
	{
		returnValue = KEY_PRESSED;
		//GS::wout << "KEY IS PRESSED" << endl;
	}
	else if((m_CurrKeyBuffer[key] == m_LastKeyBuffer[key]) && (m_LastKeyBuffer[key] == 1))
	{
		returnValue = KEY_HELD;
		//GS::wout << "KEY IS HELD" << endl;
	}
	else if((m_CurrKeyBuffer[key] != m_LastKeyBuffer[key]) && (m_LastKeyBuffer[key] == 1))
	{
		returnValue = KEY_RELEASED;
		//GS::wout << "KEY IS RELEASED" << endl;
	}
	else
	{
		returnValue = KEY_NOCHANGE;
	}

	m_LastKeyBuffer[key] = ((m_KeyBuffer[key] & 0x80) != 0);
	return returnValue;
}

bool DirectInput::keypressed(int Key)
{
	bool relevantState;

	m_CurrKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);
	
	relevantState = (m_CurrKeyBuffer[Key] != m_LastKeyBuffer[Key]) && (m_LastKeyBuffer[Key] == 0);
	
	m_LastKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);

	return relevantState;
}

bool DirectInput::keyheld(int Key)
{
	bool relevantState;

	m_CurrKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);
	
	relevantState = (m_CurrKeyBuffer[Key] == m_LastKeyBuffer[Key]) && (m_LastKeyBuffer[Key] == 1);
	
	m_LastKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);

	return relevantState;
}

bool DirectInput::keyreleased(int Key)
{
	bool relevantState = false;

	m_CurrKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);
	
	if(m_CurrKeyBuffer[Key] != m_LastKeyBuffer[Key])
	{
		relevantState = (m_CurrKeyBuffer[Key] != m_LastKeyBuffer[Key]) && (m_LastKeyBuffer[Key] == 1);
	}
	m_LastKeyBuffer[Key] = ((m_KeyBuffer[Key] & 0x80) != 0);

	return relevantState;
}



bool DirectInput::mouseButtonDown(int button) {

	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseX() {	
	return (float)mMousePos.x;
}

float DirectInput::mouseY() {
	return (float)mMousePos.y;
}


float DirectInput::mouseDX() {
	return (float)mMouseState.lX;
}

float DirectInput::mouseDY() {
	return (float)mMouseState.lY;
}

float DirectInput::mouseDZ() {
	return (float)mMouseState.lZ;
}