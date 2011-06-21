#ifndef AVATAR_H
#define AVATAR_H

#include <string>

#include "d3dUtil.h"

class GameWorld;
class SteeringBehavior;

class Avatar
{
protected:
  
	//Pointer to the GameWorld this Avatar belongs to
	GameWorld*		  m_cWorld;

	//Position
	D3DXVECTOR3		  m_vPosition;
	//Radius
	float			  m_fRadius;

	//Scale
	D3DXVECTOR3       m_vScale;

	//Magnitude of Avatar's velocity
	float			  m_fSpeed;
	//Direction of Avatar's velocity
	D3DXVECTOR3		  m_vHeading;
	D3DXQUATERNION    m_qHeading;

	//Color of this object
	D3DCOLOR		  m_cColor;

	//Text for the object
	char*			  m_cTextBuffer;

	//Number of chars used for the text
	int				  m_iUsedChars;

	//Number of lines in this object's text
	int				  m_iTextLines;

public:

	Avatar(GameWorld* gw);
	Avatar(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head);

	~Avatar(){ 
		delete m_cTextBuffer; 
	}
	
	void			  Update(float time_elapsed);

	GameWorld*		  World()const { return m_cWorld; }

	D3DXVECTOR3		  Position()const{ return m_vPosition; }
	void			  SetPos(D3DXVECTOR3 new_pos){ m_vPosition = new_pos; }
	void			  RandomPosition(int x, int y, int z){ m_vPosition.x = (float)((rand() % (2 * x)) - x); m_vPosition.y = (float)((rand() % (2 * y)) - y); }
	void			  SetPosition(float x, float y, float z){ m_vPosition = D3DXVECTOR3(x, y, z); }

	float			  Radius()const{return m_fRadius;}
	D3DXVECTOR3       Scale()const{return m_vScale;}
	void			  SetRadius(float val){ m_fRadius = val; }
	void			  SetScale(D3DXVECTOR3 scale){ m_vScale = scale; }
	void			  SetScale(float x, float y, float z){ m_vScale = D3DXVECTOR3(x, y, z); }


	D3DXVECTOR3		  Heading()const{ return m_vHeading; }
	void			  SetHeading(D3DXVECTOR3 val){ D3DXVec3Normalize( &m_vHeading, &val ); }
	void			  SetHeading(float val);
	void              SetHeading(float yaw, float pitch, float roll);
	void			  SetHeading(D3DXQUATERNION quat);

	//float			  Speed()const{ return m_fSpeed; }
	//void			  SetSpeed(float val){ m_fSpeed = val; }

	D3DCOLOR		  Color()const{ return m_cColor; }
	void			  SetColor(int arr, int gee, int bee){ m_cColor = D3DCOLOR_XRGB(arr, gee, bee); }
	void			  SetColor(D3DCOLOR col){ m_cColor = col; }

	void			  ClearText();
	void			  AddText(char* str, bool newLine = false);
	void			  AddText(char* str, int in, bool newLine = false);
	void			  AddText(char* str, float fp, bool newLine = false);
	void			  AddText(char* str, double db, bool newLine = false);

	char*			  Text()const{ return m_cTextBuffer; }
	int				  TextLines()const{ return m_iTextLines; }
	int				  CharsUsed()const{ return m_iUsedChars; }

	//Returns the angle the Avatar is facing relative to world coordinates
	float		      Direction();
	//Draws the Avatar
	virtual void	  Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix) = 0;
};

class Triangle : public Avatar
{
private:
	int m_iID;
	
public:
	static int m_iNextID;
	Triangle(GameWorld* gw, int id);
	Triangle(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head);
	void			  Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	int				  ID()const{ return m_iID; }
	int				  NextID()const{ return m_iNextID; }
};

class Square : public Avatar
{
private:
	int m_iID;
	
public:
	static int m_iNextID;
	Square(GameWorld* gw, int id);
	Square(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head);
	void			  Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	int				  ID()const{ return m_iID; }
	int				  NextID()const{ return m_iNextID; }
};

class Sphere : public Avatar
{
private:
	int m_iID;
	
	ID3DXMesh* m_mSphere;
public:
	static int m_iNextID;
	Sphere(GameWorld* gw, ID3DXMesh* sphereMesh, int id);
	Sphere(GameWorld* gw, ID3DXMesh* sphereMesh, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head);
	~Sphere(){ m_mSphere = NULL; }
	void			  Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	int				  ID()const{ return m_iID; }
	int				  NextID()const{ return m_iNextID; }
};

class Line : public Avatar
{
private:
	int m_iID;
	

	D3DXVECTOR3	m_vEndPosition;

	ID3DXLine* m_pLineDrawer;

public:
	static int m_iNextID;
	Line(GameWorld* gw, int id, ID3DXLine* lineDrawer);
	void			  Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix);

	D3DXVECTOR3		  EndPosition()const{ return m_vEndPosition; }
	void			  SetEndPos(D3DXVECTOR3 new_pos){ m_vEndPosition = new_pos; }
	void			  RandomEndPosition(int x, int y, int z){ m_vEndPosition.x = (float)((rand() % (2 * x)) - x); m_vEndPosition.y = (float)((rand() % (2 * y)) - y); }
	void			  SetEndPosition(float x, float y, float z){ m_vEndPosition = D3DXVECTOR3(x, y, z); }

	int				  ID()const{ return m_iID; }
	int				  NextID()const{ return m_iNextID; }
};

#endif