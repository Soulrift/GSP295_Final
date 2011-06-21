#include "GameWorld.h"
#include "Vertex.h"
#include <tchar.h>
#include "..//ColorDefines.h"

GameWorld::GameWorld()
{
	m_iBackRed = 255;
	m_iBackGreen = 255;
	m_iBackBlue = 255;
	SetBackground( m_iBackRed, m_iBackGreen, m_iBackBlue );

	m_bPaused = false;


	buildGeoBuffers();

	//Create main sphere to be used to draw spheres place by user
	//Create a temporary sphere first
	ID3DXMesh* tempSphere;
	HR(D3DXCreateSphere(gd3dDevice, 1.0f, 10, 16, &tempSphere, 0));
	//Clone temp sphere to the main sphere and give its vertices a color value
	HR(tempSphere->CloneMeshFVF( D3DXMESH_MANAGED, D3DFVF_XYZ | D3DFVF_DIFFUSE, gd3dDevice, &m_mSphere ));
	//Release the temp sphere
	ReleaseCOM(tempSphere);


	//Initialize line drawer object
	HR(D3DXCreateLine(gd3dDevice, &m_pLineDrawer));
	
	//Build font object
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

	ClearText();
}

void GameWorld::OnLostDevice()
{
	HR(mFont->OnLostDevice());
	HR(m_pLineDrawer->OnLostDevice());
}

void GameWorld::OnResetDevice()
{
	HR(mFont->OnResetDevice());
	HR(m_pLineDrawer->OnResetDevice());
}

void GameWorld::Release()
{
	DestroyTriangles();
	DestroySquares();
	DestroySpheres();
	DestroyLines();

	ReleaseCOM(mVB);
	ReleaseCOM(mIB);

	ReleaseCOM(m_mSphere);
	ReleaseCOM(mFont);
	ReleaseCOM(m_pLineDrawer);
}

int GameWorld::CreateTriangle(int id)
{
	Triangle* triangle = new Triangle(this, id);
	m_vTriangle.push_back(triangle);
	return triangle->ID();
}

//Returns Triangle with the given ID
Triangle* GameWorld::GetTriangle(unsigned int TriangleID)
{
	for (UINT i = 0; i < m_vTriangle.size(); i++)
		if ( m_vTriangle[i]->ID() == TriangleID )
			return m_vTriangle[i];

	assert( "Triangle not found with given ID" && false );

	return NULL;
}

void GameWorld::DestroyTriangles()
{
	//FIX by JM (2009)
	//for (UINT i = 0; i < m_vTriangle.size(); i++)
	//	delete m_vTriangle[i];

	//while(m_vTriangle.begin() != m_vTriangle.end())
	//	m_vTriangle.erase(m_vTriangle.begin());

	for(size_t i = 0; i < m_vTriangle.size(); i++)
	{
		delete m_vTriangle[i];
	}
	
	m_vTriangle.clear();

}

void GameWorld::RemoveTriangle(unsigned int TriangleID)
{
	for (UINT i = 0; i < m_vTriangle.size(); i++)
	{
		if ( m_vTriangle[i]->ID() == TriangleID )
		{
			delete m_vTriangle[i];
			m_vTriangle.erase(m_vTriangle.begin() + i);
		}
	}
}

int GameWorld::CreateSquare(int id)
{
	Square* square = new Square(this, id);
	m_vSquare.push_back(square);
	return square->ID();
}

//Returns Square with the given ID
Square* GameWorld::GetSquare(unsigned int SquareID)
{
	for (UINT i = 0; i < m_vSquare.size(); i++)
		if ( m_vSquare[i]->ID() == SquareID )
			return m_vSquare[i];

	assert( "Square not found with given ID" && false );

	return NULL;
}

void GameWorld::DestroySquares()
{
	//FIX by JM (2009)
	//for (UINT i = 0; i < m_vSquare.size(); i++)
	//	delete m_vSquare[i];

	//while(m_vSquare.begin() != m_vSquare.end())
	//	m_vSquare.erase(m_vSquare.begin());

	for(size_t i = 0; i < m_vSquare.size(); i++)
	{
		delete m_vSquare[i];
	}

	m_vSquare.clear();

}


void GameWorld::RemoveSquare(unsigned int SquareID)
{
	for (UINT i = 0; i < m_vSquare.size(); i++)
	{
		if ( m_vSquare[i]->ID() == SquareID )
		{
			delete m_vSquare[i];
			m_vSquare.erase(m_vSquare.begin() + i);
		}
	}
}

int GameWorld::CreateSphere(int id)
{
	Sphere* sphere = new Sphere(this, m_mSphere, id);
	m_vSphere.push_back(sphere);
	return sphere->ID();
}

//Returns Sphere with the given ID
Sphere* GameWorld::GetSphere(unsigned int SphereID)
{
	for (UINT i = 0; i < m_vSphere.size(); i++)
		if ( m_vSphere[i]->ID() == SphereID )
			return m_vSphere[i];

	assert( "Sphere not found with given ID" && false );

	return NULL;
}

void GameWorld::DestroySpheres()
{
	//FIX by JM (2009)
	//for (UINT i = 0; i < m_vSphere.size(); i++)
	//	delete m_vSphere[i];

	//while(m_vSphere.begin() != m_vSphere.end())
	//	m_vSphere.erase(m_vSphere.begin());

	for(size_t i = 0; i < m_vSphere.size(); i++)
	{
		delete m_vSphere[i];
	}

	m_vSphere.clear();

}

void GameWorld::RemoveSphere(unsigned int SphereID)
{
	for (UINT i = 0; i < m_vSphere.size(); i++)
	{
		if ( m_vSphere[i]->ID() == SphereID )
		{
			delete m_vSphere[i];
			m_vSphere.erase(m_vSphere.begin() + i);
		}
	}
}

int GameWorld::CreateLine(int id)
{
	Line* line = new Line(this, id, m_pLineDrawer);
	m_vLine.push_back(line);
	return line->ID();
}

//Returns Line with the given ID
Line* GameWorld::GetLine(unsigned int LineID)
{
	for (UINT i = 0; i < m_vLine.size(); i++)
		if ( m_vLine[i]->ID() == LineID )
			return m_vLine[i];

	assert( "Line not found with given ID" && false );

	return NULL;
}

void GameWorld::DestroyLines()
{
	//FIX by JM (2009)
	//for (UINT i = 0; i < m_vLine.size(); i++)
	//	delete m_vLine[i];

	while(m_vLine.begin() != m_vLine.end())
		m_vLine.erase(m_vLine.begin());
}

void GameWorld::RemoveLine(unsigned int LineID)
{
	for (UINT i = 0; i < m_vLine.size(); i++)
	{
		if ( m_vLine[i]->ID() == LineID )
		{
			delete m_vLine[i];
			m_vLine.erase(m_vLine.begin() + i);
		}
	}
}

void GameWorld::SetCurrentColor(D3DCOLOR color, bool isSphere)
{
	m_cCurrentColor = color;

	if ( isSphere )
	{
		//m_mSphere->
		VertexCol* v = 0;
		HR(m_mSphere->LockVertexBuffer(0, (void**)&v));

		int a = m_mSphere->GetNumVertices();
		a = 5;
		
		//for ( unsigned int i = 0; i < m_mSphere->GetNumVertices(); ++i )
		for ( unsigned int i = 0; i <= 152; i = i + 3 ) //give spiral effect on the ball
		{
			v[i].col = color;
			if(i != 0)
			{
				int colorDegrade = 50;
				int r = ((RGBQUAD*)&color)->rgbRed		- colorDegrade;
				int g = ((RGBQUAD*)&color)->rgbGreen	- colorDegrade;
				int b = ((RGBQUAD*)&color)->rgbBlue		- colorDegrade;
				if(r < 0) { r = 0; }
				if(g < 0) { g = 0; }
				if(b < 0) { b = 0; }

				v[i - 1].col = D3DCOLOR_XRGB(r, g, b);

				colorDegrade = 100;
				r = ((RGBQUAD*)&color)->rgbRed		- colorDegrade;
				g = ((RGBQUAD*)&color)->rgbGreen	- colorDegrade;
				b = ((RGBQUAD*)&color)->rgbBlue		- colorDegrade;
				if(r < 0) { r = 0; }
				if(g < 0) { g = 0; }
				if(b < 0) { b = 0; }

				v[i - 2].col = D3DCOLOR_XRGB(r, g, b);

				//v[i - 1].col = WHITE;
			}
		}

		HR(m_mSphere->UnlockVertexBuffer());
	}
	else
	{
		VertexCol* v = 0;
		HR(mVB->Lock(0, 0, (void**)&v, 0));

		for ( int i = 0; i < 12; i++ ) //HW
		{
			if(!(i == 1 || i == 4 || i == 5 || i == 8))
			{

				int colorDegrade = 200;

				//type case method - technically faster
				int r = ((RGBQUAD*)&color)->rgbRed		- colorDegrade;
				int g = ((RGBQUAD*)&color)->rgbGreen	- colorDegrade;
				int b = ((RGBQUAD*)&color)->rgbBlue		- colorDegrade;
				if(r < 0) { r = 0; }
				if(g < 0) { g = 0; }
				if(b < 0) { b = 0; }
				v[i].col = D3DCOLOR_XRGB(r, g, b);

				//((RGBQUAD*)&color).red

			}
			else
			{
				v[i].col = color; //HW
			}
			
		}
		HR(mVB->Unlock());
	}
}

void GameWorld::Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	// Let Direct3D know the vertex buffer, index buffer and vertex 
	// declaration we are using.
	HR(gd3dDevice->SetStreamSource(0, mVB, 0, sizeof(VertexCol)));
	HR(gd3dDevice->SetIndices(mIB));
	HR(gd3dDevice->SetVertexDeclaration(VertexCol::Decl));
	HR(gd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE));
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
    //HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
	HR(gd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD)); //hw

	D3DXVECTOR3 textLoc;
	D3DXMATRIX M, I;
	D3DXMatrixIdentity( &I );
	D3DVIEWPORT9 viewPort;
	HR(gd3dDevice->GetViewport(&viewPort));

	RECT R = {0, 0, 0, 0};

	D3DCOLOR textColor = D3DCOLOR_XRGB(0, 0, 0);
	if ( (m_iBackRed + m_iBackGreen + m_iBackBlue) < 384 )
		textColor = D3DCOLOR_XRGB(255, 255, 255);

	//Loop through and draw each square
	for (UINT i = 0; i < m_vSquare.size(); i++)
	{
		if ( m_cCurrentColor != m_vSquare[i]->Color() )
			SetCurrentColor( m_vSquare[i]->Color() );
		m_vSquare[i]->Render(FX, mhWVP, viewMatrix, projMatrix);
		
		//Get the object's screen position
		D3DXVec3Project( &textLoc, &(m_vSquare[i]->Position()), &viewPort, &projMatrix, &viewMatrix, &I );
		
		//Adjust for amount of lines in this object's text
		R.left = (LONG)textLoc.x - 20;
		R.top = (LONG)textLoc.y - (20 * ((LONG)m_vSquare[i]->TextLines() + 1));
		//Draw text to the screen above the object
		HR(mFont->DrawText(0, m_vSquare[i]->Text(), m_vSquare[i]->CharsUsed(), &R, DT_NOCLIP, textColor));
	}
	
	//Loop through each triangle
	for (UINT i = 0; i < m_vTriangle.size(); i++)
	{
		if ( m_cCurrentColor != m_vTriangle[i]->Color() )
			SetCurrentColor( m_vTriangle[i]->Color() );
		m_vTriangle[i]->Render(FX, mhWVP, viewMatrix, projMatrix);

		//Get the object's screen position
		D3DXVec3Project( &textLoc, &(m_vTriangle[i]->Position()), &viewPort, &projMatrix, &viewMatrix, &I );
		
		//Adjust for amount of lines in this object's text
		R.left = (LONG)textLoc.x - 20;
		R.top = (LONG)textLoc.y - (20 * ((LONG)m_vTriangle[i]->TextLines() + 1));
		//Draw text to the screen above the object
		HR(mFont->DrawText(0, m_vTriangle[i]->Text(), m_vTriangle[i]->CharsUsed(), &R, DT_NOCLIP, textColor));
	}

	//Loop through each sphere
	for (UINT i = 0; i < m_vSphere.size(); i++)
	{
		if ( m_cCurrentColor != m_vSphere[i]->Color() )
			SetCurrentColor( m_vSphere[i]->Color(), true );

		m_vSphere[i]->Render(FX, mhWVP, viewMatrix, projMatrix);

		D3DXVec3Project( &textLoc, &(m_vSphere[i]->Position()), &viewPort, &projMatrix, &viewMatrix, &I );

		//Adjust for amount of lines in this object's text
		R.left = (LONG)textLoc.x - 20;
		R.top = (LONG)textLoc.y - (20 * ((LONG)m_vSphere[i]->TextLines() + 1));
		//Draw text to the screen above the object
		HR(mFont->DrawText(0, m_vSphere[i]->Text(), m_vSphere[i]->CharsUsed(), &R, DT_NOCLIP, textColor));
	}

	D3DXVECTOR3 midPoint;
	//Loop through each Line
	for (UINT i = 0; i < m_vLine.size(); i++)
	{
		m_vLine[i]->Render(FX, mhWVP, viewMatrix, projMatrix);
		
		midPoint = (m_vLine[i]->Position() + m_vLine[i]->EndPosition()) * 0.5f;
		D3DXVec3Project( &textLoc, &midPoint, &viewPort, &projMatrix, &viewMatrix, &I );

		//Adjust for amount of lines in this object's text
		R.left = (LONG)textLoc.x - 20;
		R.top = (LONG)textLoc.y - (20 * ((LONG)m_vLine[i]->TextLines() + 1));
		//Draw text to the screen above the object
		HR(mFont->DrawText(0, m_vLine[i]->Text(), m_vLine[i]->CharsUsed(), &R, DT_NOCLIP, textColor));
	}

	R.left = 5;
	R.top = 60;
	HR(mFont->DrawText(0, m_cTextBuffer, -1, &R, DT_NOCLIP, textColor));
}


void GameWorld::buildGeoBuffers()
{
	// Obtain a pointer to a new vertex buffer.
	HR(gd3dDevice->CreateVertexBuffer(12 * sizeof(VertexCol), 0/*D3DUSAGE_WRITEONLY*/,
		0, D3DPOOL_MANAGED, &mVB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's vertex data.

	D3DCOLOR black = D3DCOLOR_XRGB(0, 0, 0);
	D3DCOLOR grey  = D3DCOLOR_XRGB(100, 100, 100);
	D3DCOLOR red   = D3DCOLOR_XRGB(255, 0, 0);
	D3DCOLOR green = D3DCOLOR_XRGB(0, 255, 0);
	D3DCOLOR blue  = D3DCOLOR_XRGB(0, 0, 255);

	//1green = grey;

	VertexCol* v = 0;
	HR(mVB->Lock(0, 0, (void**)&v, 0));

	v[0] = VertexCol(-1.0f,  1.0f,  1.0f, black); // Pyramid
	v[1] = VertexCol( 1.0f,  0.0f,  0.0f, green);
	v[2] = VertexCol(-1.0f, -1.0f,  1.0f, black);
	v[3] = VertexCol(-1.0f,  0.0f, -1.0f, black);

	v[4] = VertexCol(-1.0f,  1.0f,  -1.0f, green); // Cube
	v[5] = VertexCol( 1.0f,  1.0f,  -1.0f, green);
	v[6] = VertexCol( 1.0f, -1.0f,  -1.0f, black);
	v[7] = VertexCol(-1.0f, -1.0f,  -1.0f, black);
	v[8] = VertexCol(-1.0f,  1.0f,  1.0f, green);
	v[9] = VertexCol( 1.0f,  1.0f,  1.0f, black);
	v[10] = VertexCol( 1.0f, -1.0f,  1.0f, black);
	v[11] = VertexCol(-1.0f, -1.0f,  1.0f, black);


	HR(mVB->Unlock());

	// Obtain a pointer to a new index buffer.
	HR(gd3dDevice->CreateIndexBuffer(48 * sizeof(WORD), D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16, D3DPOOL_MANAGED, &mIB, 0));

	// Now lock it to obtain a pointer to its internal data, and write the
	// cube's index data.
	WORD* k = 0;

	HR(mIB->Lock(0, 0, (void**)&k, 0));

	//Pyramid
	k[0] = 0; k[1] = 1; k[2] = 2;
	k[3] = 2; k[4] = 1; k[5] = 3;
	k[6] = 3; k[7] = 1; k[8] = 0;
	k[9] = 0; k[10] = 2; k[11] = 3;

	//Cube
	k[12] = 4; k[13] = 5; k[14] = 7; //Front
	k[15] = 7; k[16] = 5; k[17] = 6; //Front
	k[18] = 8; k[19] = 4; k[20] = 11; //Left
	k[21] = 11; k[22] = 4; k[23] = 7; //Left
	k[24] = 8; k[25] = 9; k[26] = 4; //Top
	k[27] = 4; k[28] = 9; k[29] = 5; //Top
	k[30] = 5; k[31] = 9; k[32] = 6; //Right
	k[33] = 6; k[34] = 9; k[35] = 10; //Right
	k[36] = 6; k[37] = 10; k[38] = 7; //Bottom
	k[39] = 7; k[40] = 10; k[41] = 11; //Bottom
	k[42] = 9; k[43] = 8; k[44] = 10; //Back
	k[45] = 10; k[46] = 8; k[47] = 11; //Back

	HR(mIB->Unlock());
}

void GameWorld::ClearText()
{
	m_iUsedChars = sprintf_s( m_cTextBuffer, 512, "");
}

//Adds specified string to the text to write each frame
void GameWorld::AddText(char* str)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 512 - m_iUsedChars, "%s", str );
}

//Adds specified string to the text to write each frame
void GameWorld::AddText(char* str, int in)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 512 - m_iUsedChars, "%s %d", str, in );
}

//Adds specified string to the text to write each frame
void GameWorld::AddText(char* str, float fp)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 512 - m_iUsedChars, "%s %f", str, fp );
}

//Adds specified string to the text to write each frame
void GameWorld::AddText(char* str, double db)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 512 - m_iUsedChars, "%s %f", str, db );
}