#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <cassert>
#include <vector>
#include <string>
#include "d3dApp.h"

#include "Avatar.h"

class GameWorld
{ 
private:

	//Background color
	D3DCOLOR m_cBackground;

	//Individual background color components
	int m_iBackRed;
	int m_iBackGreen;
	int m_iBackBlue;

	//Mesh for drawing spheres
	ID3DXMesh* m_mSphere;

	//Vectors to hold pointers to all of our objects
	std::vector<Triangle*> m_vTriangle;
	std::vector<Square*> m_vSquare;
	std::vector<Sphere*> m_vSphere;
	std::vector<Line*> m_vLine;

	//Color the current/last object is drawn in
	D3DCOLOR m_cCurrentColor;

	//Vertex and index information for triangles and squares
	IDirect3DVertexBuffer9* mVB;
	IDirect3DIndexBuffer9*  mIB;

	//Builds the vertex and index buffers
	void buildGeoBuffers();

	//Set the color to draw the current object
	void SetCurrentColor(D3DCOLOR color, bool isSphere = false);

	//Font object
	ID3DXFont* mFont;

	ID3DXSprite* mSprite;

	//Char buffer to hold the text
	char m_cTextBuffer[512];

	//Amount of text that has been used in the buffer
	int m_iUsedChars;

	//Line drawer object
	ID3DXLine* m_pLineDrawer;

	//Is the world paused?
	bool m_bPaused;

public:
  
	GameWorld();
	virtual ~GameWorld(){}

	void OnLostDevice();
	void OnResetDevice();

	void Release();

	void Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);
	virtual void Update(float time_elapsed) = 0;

	virtual void KeyPress(WPARAM keyCode) = 0;

	//Pauses/unpauses the game world, updates will not be processed while paused.
	void PauseWorld() { m_bPaused = !m_bPaused; }

	//Returns true if world is paused.
	bool Paused() const { return m_bPaused; }

	D3DCOLOR Background() const { return m_cBackground; }

	template <class T>
	void SetBackground( T arr, T gee, T bee ) { m_iBackRed = (int)arr; m_iBackGreen = (int)gee; m_iBackBlue = (int)bee; m_cBackground = D3DCOLOR_XRGB(arr, gee, bee); }
	
	int BackgroundRed() const { return m_iBackRed; }
	int BackgroundGreen() const { return m_iBackGreen; }
	int BackgroundBlue() const { return m_iBackBlue; }

	template <class T>
	void SetBackgroundRed(T val) { m_iBackRed = (int)val; m_cBackground = D3DCOLOR_XRGB(m_iBackRed, m_iBackGreen, m_iBackBlue); }
	template <class T>
	void SetBackgroundGreen(T val) { m_iBackGreen = (int)val; m_cBackground = D3DCOLOR_XRGB(m_iBackRed, m_iBackGreen, m_iBackBlue); }
	template <class T>
	void SetBackgroundBlue(T val) { m_iBackBlue = (int)val; m_cBackground = D3DCOLOR_XRGB(m_iBackRed, m_iBackGreen, m_iBackBlue); }



//Methods to do things with Triangles and Squares
//Triangles*************************************************************************************************************************

	//Adds a new Triangle to the world
	int CreateTriangle(int id = -1);
	//Returns Triangle with the given ID
	Triangle* GetTriangle(unsigned int TriangleID);
	//Destroys all Triangles
	void DestroyTriangles();
	//Removes the specified Triangle
	void RemoveTriangle(unsigned int TriangleID);
	////Updates all the Triangles
	//void UpdateTriangles(float time_elapsed);
	//Returns the number of Triangles that currently exist in this world
	int TriangleCount(){ return (int)m_vTriangle.size(); }

	//Sets the Triangle's scale
	template <class T>
	void SetTriangleScale(unsigned int TriangleID, T scale){ GetTriangle(TriangleID)->SetRadius(scale); }
	//Returns the Triangle's scale
	float GetTriangleScale(unsigned int TriangleID){ return GetTriangle(TriangleID)->Radius(); }

	//Places Triangle at the specified point
	template <class T>
	void SetTrianglePosition(unsigned int TriangleID, T x, T y, T z){ GetTriangle(TriangleID)->SetPosition((float)x, (float)y, (float)z); }
	//Returns x value of the Triangle's position
	float GetTrianglePositionX(unsigned int TriangleID){ return GetTriangle(TriangleID)->Position().x; }
	//Returns y value of the Triangle's position
	float GetTrianglePositionY(unsigned int TriangleID){ return GetTriangle(TriangleID)->Position().y; }
	//Returns y value of the Triangle's position
	float GetTrianglePositionZ(unsigned int TriangleID){ return GetTriangle(TriangleID)->Position().z; }

	//Sets the Triangle's heading to the specified angle (in radians)
	template <class T>
	void SetTriangleHeading(unsigned int TriangleID, T angle){ GetTriangle(TriangleID)->SetHeading(angle); }
	template <class T>
	void SetTriangleHeading(unsigned int TriangleID, T yaw, T pitch, T roll){ GetTriangle(TriangleID)->SetHeading((float)yaw, (float)pitch, (float)roll); }
	//Return Triangle heading
	float GetTriangleHeading(unsigned int TriangleID){ return GetTriangle(TriangleID)->Direction(); }

	//Sets the Triangle's color to the specified color
	void SetTriangleColor(unsigned int TriangleID, int arr, int gee, int bee){ GetTriangle(TriangleID)->SetColor(arr, gee, bee); }

	//Clears text buffer for this Triangle
	void ClearTriangleText(unsigned int TriangleID){ GetTriangle(TriangleID)->ClearText(); }
	//Add the specified string to the Triangle's text
	void AddTriangleText(unsigned int TriangleID, char* str){ GetTriangle(TriangleID)->AddText(str); }
	void AddTriangleText(unsigned int TriangleID, char* str, int in){ GetTriangle(TriangleID)->AddText(str, in); }
	void AddTriangleText(unsigned int TriangleID, char* str, float fp){ GetTriangle(TriangleID)->AddText(str, fp); }
	void AddTriangleText(unsigned int TriangleID, char* str, double db){ GetTriangle(TriangleID)->AddText(str, db); }

	//Returns the ID of the last Triangle created
	int GetLastTriangleID(){ if (m_vTriangle.size() > 0) return m_vTriangle[m_vTriangle.size() - 1]->ID(); else return -1; }

	//Squares*************************************************************************************************************************

	//Adds a new Square to the world
	int CreateSquare(int id = -1);
	//Returns Square with the given ID
	Square* GetSquare(unsigned int SquareID);
	//Destroys all Squares
	void DestroySquares();
	//Removes the specified Square
	void RemoveSquare(unsigned int SquareID);
	////Updates all the Squares
	//void UpdateSquares(float time_elapsed);
	//Returns the number of Squares that currently exist in this world
	int SquareCount(){ return (int)m_vSquare.size(); }

	//Sets the Square's scale
	template <class T>
	void SetSquareScale(unsigned int SquareID, T x, T y, T z){ GetSquare(SquareID)->SetScale(x, y, z); }
	//void SetSquareScale(unsigned int SquareID, T scale){ GetSquare(SquareID)->SetRadius(scale); }
	//Returns the Square's scale
	float GetSquareScale(unsigned int SquareID){ return GetSquare(SquareID)->Radius(); }

	//Places Square at the specified point
	template <class T>
	void SetSquarePosition(unsigned int SquareID, T x, T y, T z){ GetSquare(SquareID)->SetPosition((float)x, (float)y, (float)z); }
	//Returns x value of the Square's position
	float GetSquarePositionX(unsigned int SquareID){ return GetSquare(SquareID)->Position().x; }
	//Returns y value of the Square's position
	float GetSquarePositionY(unsigned int SquareID){ return GetSquare(SquareID)->Position().y; }
	//Returns y value of the Square's position
	float GetSquarePositionZ(unsigned int SquareID){ return GetSquare(SquareID)->Position().z; }

	//Sets the Squares heading to the specified angle (in radians)
	template <class T>
	void SetSquareHeading(unsigned int SquareID, T angle){ GetSquare(SquareID)->SetHeading((float)angle); }
	template <class T>
	void SetSquareHeading(unsigned int SquareID, T yaw, T pitch, T roll){ GetSquare(SquareID)->SetHeading((float)yaw, (float)pitch, (float)roll); }

	//Return Square heading
	float GetSquareHeading(unsigned int SquareID){ return GetSquare(SquareID)->Direction(); }

	void SetSquareColor(unsigned int SquareID, int arr, int gee, int bee){ GetSquare(SquareID)->SetColor(arr, gee, bee); }

	//Clears text buffer for this Square
	void ClearSquareText(unsigned int SquareID){ GetSquare(SquareID)->ClearText(); }
	//Add the specified string to the Square's text
	void AddSquareText(unsigned int SquareID, char* str){ GetSquare(SquareID)->AddText(str); }
	void AddSquareText(unsigned int SquareID, char* str, int in){ GetSquare(SquareID)->AddText(str, in); }
	void AddSquareText(unsigned int SquareID, char* str, float fp){ GetSquare(SquareID)->AddText(str, fp); }
	void AddSquareText(unsigned int SquareID, char* str, double db){ GetSquare(SquareID)->AddText(str, db); }

	//Returns the ID of the last Square created
	int GetLastSquareID(){ if (m_vSquare.size() > 0) return m_vSquare[m_vSquare.size() - 1]->ID(); else return -1; }

//Spheres*************************************************************************************************************************

	//Adds a new Sphere to the world
	int CreateSphere(int id = -1);
	//Returns Sphere with the given ID
	Sphere* GetSphere(unsigned int SphereID);
	//Destroys all Spheres
	void DestroySpheres();
	//Removes the specified Sphere
	void RemoveSphere(unsigned int SphereID);
	////Updates all the Spheres
	//void UpdateSpheres(float time_elapsed);
	//Returns the number of Spheres that currently exist in this world
	int SphereCount(){ return (int)m_vSphere.size(); }

	//Sets the Sphere's scale
	template <class T>
	void SetSphereScale(unsigned int SphereID, T scale){ GetSphere(SphereID)->SetScale(scale, scale, scale); }
	//Returns the Sphere's scale
	float GetSphereScale(unsigned int SphereID){ return GetSphere(SphereID)->Radius(); }

	//Places Sphere at the specified point
	template <class T>
	void SetSpherePosition(unsigned int SphereID, T x, T y, T z){ GetSphere(SphereID)->SetPosition((float)x, (float)y, (float)z); }
	//Returns x value of the Sphere's position
	float GetSpherePositionX(unsigned int SphereID){ return GetSphere(SphereID)->Position().x; }
	//Returns y value of the Sphere's position
	float GetSpherePositionY(unsigned int SphereID){ return GetSphere(SphereID)->Position().y; }
	//Returns y value of the Sphere's position
	float GetSpherePositionZ(unsigned int SphereID){ return GetSphere(SphereID)->Position().z; }


	void SetSphereColor(unsigned int SphereID, int arr, int gee, int bee){ GetSphere(SphereID)->SetColor(arr, gee, bee); }

	//Clears text buffer for this Sphere
	void ClearSphereText(unsigned int SphereID){ GetSphere(SphereID)->ClearText(); }
	//Add the specified string to the Sphere's text
	void AddSphereText(unsigned int SphereID, char* str){ GetSphere(SphereID)->AddText(str); }
	void AddSphereText(unsigned int SphereID, char* str, int in){ GetSphere(SphereID)->AddText(str, in); }
	void AddSphereText(unsigned int SphereID, char* str, float fp){ GetSphere(SphereID)->AddText(str, fp); }
	void AddSphereText(unsigned int SphereID, char* str, double db){ GetSphere(SphereID)->AddText(str, db); }

	//Returns the ID of the last Sphere created
	int GetLastSphereID(){ if (m_vSphere.size() > 0) return m_vSphere[m_vSphere.size() - 1]->ID(); else return -1; }

	//Lines*************************************************************************************************************************

	//Adds a new Line to the world
	int CreateLine(int id = -1);
	//Returns Line with the given ID
	Line* GetLine(unsigned int LineID);
	//Destroys all Lines
	void DestroyLines();
	//Removes the specified Line
	void RemoveLine(unsigned int LineID);
	////Updates all the Lines
	//void UpdateLines(float time_elapsed);
	//Returns the number of Lines that currently exist in this world
	int LineCount(){ return (int)m_vLine.size(); }

	//Starts the line at the specified point
	template <class T>
	void SetLineStartPoint(unsigned int LineID, T x, T y, T z){ GetLine(LineID)->SetPosition((float)x, (float)y, (float)z); }
	//Returns x value of the Line's position
	float GetLineStartPointX(unsigned int LineID){ return GetLine(LineID)->Position().x; }
	//Returns y value of the Line's position
	float GetLineStartPointY(unsigned int LineID){ return GetLine(LineID)->Position().y; }
	//Returns z value of the Line's position
	float GetLineStartPointZ(unsigned int LineID){ return GetLine(LineID)->Position().z; }


	//Ends the line at the specified point
	template <class T>
	void SetLineEndPoint(unsigned int LineID, T x, T y, T z){ GetLine(LineID)->SetEndPosition((float)x, (float)y, (float)z); }
	//Returns x value of the Line's position
	float GetLineEndPointX(unsigned int LineID){ return GetLine(LineID)->EndPosition().x; }
	//Returns y value of the Line's position
	float GetLineEndPointY(unsigned int LineID){ return GetLine(LineID)->EndPosition().y; }
	//Returns z value of the Line's position
	float GetLineEndPointZ(unsigned int LineID){ return GetLine(LineID)->EndPosition().z; }

	void SetLineColor(unsigned int LineID, int arr, int gee, int bee){ GetLine(LineID)->SetColor(arr, gee, bee); }

	//Clears text buffer for this Line
	void ClearLineText(unsigned int LineID){ GetLine(LineID)->ClearText(); }
	//Add the specified string to the Line's text
	void AddLineText(unsigned int LineID, char* str){ GetLine(LineID)->AddText(str); }
	void AddLineText(unsigned int LineID, char* str, int in){ GetLine(LineID)->AddText(str, in); }
	void AddLineText(unsigned int LineID, char* str, float fp){ GetLine(LineID)->AddText(str, fp); }
	void AddLineText(unsigned int LineID, char* str, double db){ GetLine(LineID)->AddText(str, db); }

	//Returns the ID of the last Line created
	int GetLastLineID(){ if (m_vLine.size() > 0) return m_vLine[m_vLine.size() - 1]->ID(); else return -1; }


//Text
	//Adds specified string to the text to write each frame
	void AddText(char* str);
	//Adds specified string to the text to write each frame
	void AddText(char* str, int in);
	//Adds specified string to the text to write each frame
	void AddText(char* str, float fp);
	//Adds specified string to the text to write each frame
	void AddText(char* str, double db);

	//Clears text buffer
	void ClearText();
};


#endif