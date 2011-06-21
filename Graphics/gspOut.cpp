#define IDC_MAIN_EDIT 101 //create the resource for the edit control

#include "gspOut.h"

stringstream gspOut::sGspStream;
HWND gspOut::sHwndEdit;
bool gspOut::setupVar = gspOut::setupVarFunction();

//gspOut* GLOBAL_GSP_OUTPUT_WINDOW = new gspOut(true, "Global Output Window"); 
gspOut windowptr(true, "Global Output Window");
gspOut* GLOBAL_GSP_OUTPUT_WINDOW = &windowptr;


//default constructor
gspOut::gspOut(void)
{
	showDebug = true; //do not show box by default
	title = "GSP Output Debugger";

	currentBounce = 0.0f;

	clear();

	//InitializeCriticalSection(&outputLock);
}

//parameterized constructor
gspOut::gspOut(bool showWindow, string getTitle) {
	title = getTitle;

	//do not show information unless set to true;
	if(showWindow) {
		showDebug = true;
		gsp_CreateWindow();
	}

	currentBounce = 0.0f;

	clear();

	//InitializeCriticalSection(&outputLock);
}
//destructor
gspOut::~gspOut(void)
{
	//empty, object will be deleted when the program is closed
	//DeleteCriticalSection(&outputLock);
}

void gspOut::clear() {
	gsp_stream.str("");
}

void gspOut::printD3DXVec3(D3DXVECTOR3 vec, string identifier, float rate, float dt, bool clear)
{
	if(currentBounce < 0.0f)
	{
		currentBounce = rate;
		printD3DXVec3(vec, identifier, clear);
	}

	currentBounce -= dt;
}

void gspOut::printD3DXQuat(D3DXQUATERNION quat, string identifier, float rate, float dt, bool clear)
{
	if(currentBounce < 0.0f)
	{
		currentBounce = rate;
		printD3DXQuat(quat, identifier, clear);
	}

	currentBounce -= dt;
}

void gspOut::printD3DXVec3(D3DXVECTOR3 vec, string identifier, bool cl)
{
	if(cl) { clear(); };
	(*this) << identifier << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << endl;
}

void gspOut::printD3DXQuat(D3DXQUATERNION quat, string identifier, bool cl)
{
	if(cl) { clear(); };
	(*this) << identifier << "(" << quat.w << ", " << quat.x << ", " << quat.y << ", " << quat.z << ")" << endl;
}

//start the function parameter
void gspOut::gsp_CreateWindow()
{
	static bool firstRun = true;

	//start windows parameters
	WNDCLASSEX    wc; 
	::ZeroMemory (&wc, sizeof(WNDCLASSEX)); //zeros out the memory block to make room for a new window
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.hInstance     = instanceMain;
	wc.lpfnWndProc   = gspOut::WndProc;
	wc.lpszClassName = "gspOutGFT";

	//check to see if the window was registered
	RegisterClassEx(&wc);
	/*if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return;
	}*/

	//get the size of the screen in order to properly place th window on the right side
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

  //create the window object
	hwndMain = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"gspOutGFT",
		title.c_str(), //window title, can be user specified
		WS_SIZEBOX | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
		(screenWidth-500), 10, 500, (screenHeight-50),
		NULL, NULL, instanceMain, this);

  //test to see if hwnd is properly initialized, and the user wants to show the debug
	if(showDebug && hwndMain) {
		ShowWindow(hwndMain, SW_SHOWNORMAL);
		UpdateWindow(hwndMain);
	}
}

//static function for callback
LRESULT CALLBACK gspOut::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	//begin process of routing to the non-static callback function
	if (msg == WM_NCCREATE)
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));

	//pointer object based off of the current window and point to the non-static callback
	gspOut* base = reinterpret_cast<gspOut*>( GetWindowLongPtr(hwnd, GWLP_USERDATA) );

	if (!base) return DefWindowProc(hwnd,msg,wParam,lParam);
	else return base->WndProcNS(hwnd,msg,wParam,lParam);
}

//non-static message handler
LRESULT CALLBACK gspOut::WndProcNS(HWND gspHWND, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_CREATE:
		{
			HFONT hfDefault; //use the default device font
			//create the edit control as a child of the main window
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY, 
				0, 0, 100, 100, gspHWND, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
			//check to see if the edit control was created properly
			if(hEdit == NULL)
				MessageBox(gspHWND, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		}
		break;
		case WM_SIZE:
		{
			//function to resize the edit control when the user adjust the main window
			RECT rcClient;
			GetClientRect(gspHWND, &rcClient);
			hEdit = GetDlgItem(gspHWND, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}
		break;          
		case WM_CLOSE: {
			DestroyWindow(gspHWND); //destroy window when closed
		}
		break;
		case WM_DESTROY: {
			PostQuitMessage(0); //end program
		}
		break;
	}
	return DefWindowProc(gspHWND, uMsg, wParam, lParam); //default - To ensure every message is processed and not ignored.
}

//being parse function
string gspOut::parseEndl(stringstream &source) {
	string searchString("\n"); //find string
	string replaceString("\r\n"); //replace srting
	string temp = source.str(); //create temp string from stringstream

    string::size_type pos = 0; //set init position
	//begin string parsing loop to replace \n with \r\n
    while ((pos = temp.find(searchString, pos)) != string::npos ) {
        temp.replace( pos, searchString.size(), replaceString);
        pos += replaceString.size();
    }
    return temp; //return string type
}