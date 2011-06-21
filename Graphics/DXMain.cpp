#include "d3dApp.h"
#include "Vertex.h"
#include <crtdbg.h>
#include <list>
#include <tchar.h>

#include "..\MyGameWorld.h"

 

class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~Game();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();

	//Handles mouse/keyboard input from windows messages
	void EventKeyPress(WPARAM wParam, LPARAM lParam);
	void EventMouseMove(WPARAM wParam, LPARAM lParam);
	void EventMouseWheel(WPARAM wParam, LPARAM lParam);
	void EventLeftButton(WPARAM wParam, LPARAM lParam, bool lmbDown);
	void EventRightButton(WPARAM wParam, LPARAM lParam, bool rmbDown);

	// Helper methods

	void buildFX();
	void buildViewMtx();
	void buildProjMtx();

	void updateGfxStats(float dt);
	void displayGfxStats();

private:
	ID3DXEffect*            mFX;
	D3DXHANDLE              mhTech;
	D3DXHANDLE              mhWVP;

	D3DXMATRIX mProj;

	bool mCursorHidden;
	float mMouseDivisor;

	ID3DXFont* mFont;
	ID3DXSprite* mSprite;

	float mFPS;
	float mMilliSecPerFrame;
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
		//_CrtSetBreakAlloc(145);
	#endif

	Game app(hInstance, "Game", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	gDInput = &di;

    if(!gd3dApp->checkDeviceCaps())
		return 0;
	else
		return gd3dApp->run();
}

Game::Game(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	gMyGameWorld = new MyGameWorld();
	gMyGameWorld->Initialize();

	mFPS = 0.0f;
	mMilliSecPerFrame = 0.0f;

	D3DXFONT_DESC fontDesc;
	fontDesc.Height          = 18;
    fontDesc.Width           = 0;
    fontDesc.Weight          = 0;
    fontDesc.MipLevels       = 1;
    fontDesc.Italic          = false;
    fontDesc.CharSet         = DEFAULT_CHARSET;
    fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
    fontDesc.Quality         = DEFAULT_QUALITY;
    fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
    strcpy_s(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	if(!SUCCEEDED(D3DXCreateSprite(gd3dDevice, &mSprite)))
	{
		MessageBox(0, "Sprite Creation Failed", "Uh Oh", MB_ICONEXCLAMATION);
	}

	buildFX();

	onResetDevice();

	InitAllVertexDeclarations();
}

Game::~Game()
{
	gMyGameWorld->Release();
	delete gMyGameWorld;

	ReleaseCOM(mFX);
	ReleaseCOM(mFont);

	DestroyAllVertexDeclarations();

	delete DXPI;
	delete DXCI;
}

bool Game::checkDeviceCaps()
{
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	// Check for vertex shader version 2.0 support.
	if( caps.VertexShaderVersion < D3DVS_VERSION(2, 0) )
		return false;

	// Check for pixel shader version 2.0 support.
	if( caps.PixelShaderVersion < D3DPS_VERSION(2, 0) )
		return false;

	return true;
}

void Game::onLostDevice()
{
	HR(mFX->OnLostDevice());
	HR(mFont->OnLostDevice());

	gMyGameWorld->OnLostDevice();
	mSprite->OnLostDevice();
}

void Game::onResetDevice()
{
	HR(mFX->OnResetDevice());
	HR(mFont->OnResetDevice());

	gMyGameWorld->OnResetDevice();
	mSprite->OnResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void Game::updateScene(float dt)
{
	
	updateGfxStats(dt);
	
	//Update the game world if it is not paused.
	if ( !gMyGameWorld->Paused() )
		gMyGameWorld->Update(dt);

	
}


void Game::drawScene()
{

	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, gMyGameWorld->Background(), 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	// Setup the rendering FX
	HR(mFX->SetTechnique(mhTech));
	
	//gMyGameWorld->Render(mFX, mhWVP, mView, mProj);
	gMyGameWorld->Render(mFX, mhWVP, DXCI->getView(), mProj);

	displayGfxStats();

	//sprite
	mSprite->Begin(D3DXSPRITE_ALPHABLEND);

	mSprite->Flush();
	mSprite->End();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));


}
 
void Game::buildFX()
{
	// Create the FX from a .fx file.
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, "color.fx", 
		0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors));
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	// Obtain handles.
	mhTech = mFX->GetTechniqueByName("ColorTech");
	mhWVP  = mFX->GetParameterByName(0, "gWVP");
}

void Game::buildProjMtx()
{
	//Camera's Projection matrix
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}

void Game::updateGfxStats(float dt)
{
	// Make static so that their values persist accross function calls.
	static float numFrames   = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numFrames += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		mFPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		mMilliSecPerFrame = 1000.0f / mFPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames   = 0.0f;
	}
}

void Game::displayGfxStats()
{
	// Make static so memory is not allocated every frame.
	static char buffer[256];

	sprintf_s(buffer, "Frames Per Second = %.2f\n" "Milliseconds Per Frame = %.4f\n", mFPS, mMilliSecPerFrame);

	D3DCOLOR textColor = D3DCOLOR_XRGB(0, 0, 0);
	if ( gMyGameWorld->BackgroundRed() + gMyGameWorld->BackgroundGreen() + gMyGameWorld->BackgroundBlue() < 384 )
		textColor = D3DCOLOR_XRGB(255, 255, 255);

	RECT R = {5, 5, 0, 0};
	HR(mFont->DrawText(0, buffer, -1, &R, DT_NOCLIP, textColor));
}

void Game::EventMouseMove(WPARAM wParam, LPARAM lParam)
{

}

void Game::EventMouseWheel(WPARAM wParam, LPARAM lParam)
{
	
}

void Game::EventKeyPress(WPARAM wParam, LPARAM lParam)
{
	//Toggle full screen mode when escape is pressed
	if( wParam == VK_ESCAPE )
		enableFullScreenMode(md3dPP.Windowed);

	gMyGameWorld->KeyPress(wParam);
}


void Game::EventLeftButton(WPARAM wParam, LPARAM lParam, bool lmbDown)
{
	m_bRMBDown = lmbDown;

	m_iMouseX = LOWORD(lParam);
	m_iMouseY = HIWORD(lParam);
}


void Game::EventRightButton(WPARAM wParam, LPARAM lParam, bool rmbDown)
{
	m_bRMBDown = rmbDown;

	m_iMouseX = LOWORD(lParam);
	m_iMouseY = HIWORD(lParam);
}