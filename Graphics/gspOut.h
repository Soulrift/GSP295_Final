/*
Program created by Cameron Ferguson
Date: November 14, 2010
Version: 1.0.3
Description: Create an output window to display user output variables to debug their program with ease
*/

#ifndef GSPOUT_H
#define GSPOUT_H

#include <windows.h> //included for the creation of windows. Standard winapi header file.
#include <sstream> //stringstream object to hold all of the text
#include <process.h> //included for the multithreading aspect of the program
#include <d3dx9math.h>

using namespace std;

//begin gspOut class
class gspOut
{
public:
	gspOut(void); //default constructor
	gspOut(bool, string); //parameterized
	~gspOut(void); //destructor

	//template for using the stream operator in a stringstream format..
	template <class T>
	gspOut & operator <<(const T &rhs) { //template to accept textual input, such as numbers, chars, strings, etc
		gsp_stream << rhs;
		gspOut::sGspStream.str("");
		gspOut::sGspStream << parseEndl(gsp_stream);
		gspOut::sHwndEdit = hEdit;
		
		//_beginthread(writeOutputThread, 0, (void*) 12 ); //use another thread for more advance calculations
		SetWindowText(gspOut::sHwndEdit, gspOut::sGspStream.str().c_str()); //post to the window
		SendMessage(gspOut::sHwndEdit, WM_VSCROLL, SB_BOTTOM, NULL);
		return *this;
	}

	//template to accept functions from the stream, such as endl
	gspOut & operator<<(ostream& (*gspPF)(ostream&)) { 
		gspPF(gsp_stream);
		return *this;
	}

	void clear(); //clears the screen

	void printD3DXVec3(D3DXVECTOR3 vec, string identifier, float rate, float dt, bool clear);
	void printD3DXVec3(D3DXVECTOR3 vec, string identifier, bool clear);
	void printD3DXQuat(D3DXQUATERNION quat, string identifier, bool clear);
	void printD3DXQuat(D3DXQUATERNION quat, string identifier, float rate, float dt, bool clear);

	static stringstream sGspStream;
	static HWND sHwndEdit;
	static bool setupVar;
	static bool setupVarFunction() {
		gspOut::sGspStream.str("");
		gspOut::sHwndEdit = NULL;
		return true;
	}

private:
	string parseEndl(stringstream &); //function to parse the endl \n to use \r\n for the edit control
	
	LRESULT CALLBACK WndProcNS(HWND, UINT, WPARAM, LPARAM); //non-static window callback
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //static window callback to handle the re-routing of the CALLBACK function

	//window frame variables
	HINSTANCE instanceMain; //create the instance of the main window
	HWND hwndMain, hEdit; //handle window vars. hwnd_ = main window, hEdit = edit control

	stringstream gsp_stream; //create the stringstream object to contrain the output

	bool showDebug; //to show the debug window or not
	string title; //create the custom title of the debugger window
	void gsp_CreateWindow(); //function used to create the propertise of the window

	float currentBounce;

	CRITICAL_SECTION outputLock;

};

#define wout (*GLOBAL_GSP_OUTPUT_WINDOW)
extern gspOut* GLOBAL_GSP_OUTPUT_WINDOW;

#endif