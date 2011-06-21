#include "Avatar.h"
#include "GameWorld.h"
#include "gspOut.h"

int Triangle::m_iNextID = 0;
int Square::m_iNextID = 0;
int Sphere::m_iNextID = 0;
int Line::m_iNextID = 0;

Avatar::Avatar(GameWorld* gw)
	  : m_cWorld(gw),
		m_fRadius(1.0f),
		m_vPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
		m_vHeading(D3DXVECTOR3(1.0f, 0.0f, 0.0f)),
		m_fSpeed(0.0f),
		m_cColor(D3DCOLOR_XRGB(0, 0, 0))
{
	m_vScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_qHeading = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f),
	m_cTextBuffer = new char[256];
	ClearText();
}

Avatar::Avatar(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head)
	  : m_cWorld(gw),
		m_fRadius(radius),
		m_vPosition(pos),
		m_vHeading(head),
		m_fSpeed(0.0f),
		m_cColor(D3DCOLOR_XRGB(0, 0, 0))
{
	m_vScale = D3DXVECTOR3(radius, radius, radius); 
	m_qHeading = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f),
	m_cTextBuffer = new char[256];
	ClearText();
}

void Avatar::Update(float time_elapsed)
{
	//m_vPosition += m_vHeading * m_fSpeed * time_elapsed;
}

void Avatar::SetHeading(float val)
{
	D3DXMATRIX R;
	D3DXVECTOR3 A(1.0f, 0.0f, 0.0f);

	D3DXMatrixRotationZ(&R, val);

	D3DXVec3TransformCoord(&m_vHeading, &A, &R);
}

void Avatar::SetHeading(float yaw, float pitch, float roll)
{
//	D3DXMATRIX R; //HW
//	D3DXVECTOR3 A(1.0f, 0.0f, 0.0f);
//	char message[100];

	D3DXQuaternionRotationYawPitchRoll(&m_qHeading, yaw, pitch, roll);
//	D3DXMatrixRotationYawPitchRoll(&R, yaw, pitch, roll);

//	D3DXVec3TransformCoord(&m_vHeading, &A, &R);
//	sprintf(message, "Yaw: %0.02f Pitch: %0.02f Roll: %0.02f\n", yaw, pitch, roll);
//	this->ClearText();
//	this->AddText(message);
//	sprintf(message, "X: %0.02f Y: %0.02f z: %0.02f\n", m_vHeading.x, m_vHeading.y, m_vHeading.z);
//	this->AddText(message);
}

void Avatar::SetHeading(D3DXQUATERNION quat)
{
	m_qHeading = quat;
}

float Avatar::Direction()
{ 
	D3DXVECTOR3 A(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 B = m_vHeading;

	if ( A == B )
	{
		return 0;
	}

	float lengthA = D3DXVec3Length( &A );
	float lengthB = D3DXVec3Length( &B );

	assert( ( lengthA > 0 ) && ( lengthB > 0 ) && "AngleBetweenVectors given an zero length vector" );

	//Find the arccosine of the dot product of the two vectors divided by the product of their magnitudes
	float theta = acos( D3DXVec3Dot( &A, &B ));

	//The previous step will always return a positive number, so we check to see if the angle is negative.
	//  If so, we flip it.
	if (A.y * B.x > A.x * B.y)
		theta *= -1;

	return theta;
}

void Avatar::ClearText()
{
	m_iUsedChars = sprintf_s( m_cTextBuffer, 256, "");
	m_iTextLines = 1;
}

//Adds specified string to the text to write each frame
void Avatar::AddText(char* str, bool newLine)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 256 - m_iUsedChars, "%s", str );
	if ( newLine )
		m_iTextLines++;
}

//Adds specified string to the text to write each frame
void Avatar::AddText(char* str, int in, bool newLine)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 256 - m_iUsedChars, "%s %d", str, in );
	if ( newLine )
		m_iTextLines++;
}

//Adds specified string to the text to write each frame
void Avatar::AddText(char* str, float fp, bool newLine)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 256 - m_iUsedChars, "%s %f", str, fp );
	if ( newLine )
		m_iTextLines++;
}

//Adds specified string to the text to write each frame
void Avatar::AddText(char* str, double db, bool newLine)
{
	m_iUsedChars += sprintf_s( m_cTextBuffer + m_iUsedChars, 256 - m_iUsedChars, "%s %f", str, db );
	if ( newLine )
		m_iTextLines++;
}


Triangle::Triangle(GameWorld* gw, int id) : Avatar(gw)
{
	//assert( (( id >= m_iNextID ) || ( id == -1 )) && "Triangle ID must be greater than the ID of the last triangle created");
	if ( id == -1 )
		m_iID = m_iNextID++;
	else
	{
		m_iID = id;
		m_iNextID = id + 1;
	}
}

Triangle::Triangle(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head)
	    : Avatar(gw, radius, pos, head)
{
}

void Triangle::Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	D3DXMATRIX T, R, S;
//	D3DXVECTOR3 H;
	//char message[200];

	// Begin passes.
	UINT numPasses = 0;
	HR(FX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(FX->BeginPass(i));

		D3DXMatrixScaling(&S, Scale().x, Scale().y, Scale().z);
		D3DXMatrixRotationQuaternion(&R, &m_qHeading);
		D3DXMatrixTranslation(&T, Position().x, Position().y, Position().z);

		HR(FX->SetMatrix(mhWVP, &(S*R*T*viewMatrix*projMatrix)));
		HR(FX->CommitChanges());
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 4));

		HR(FX->EndPass());
	}
	HR(FX->End());
}


Square::Square(GameWorld* gw, int id) : Avatar(gw)
{
	//assert( (( id >= m_iNextID ) || ( id == -1 )) && "Square ID must be greater than the ID of the last square created");
	if ( id == -1 )
		m_iID = m_iNextID++;
	else
	{
		m_iID = id;
		m_iNextID = id + 1;
	}
}

Square::Square(GameWorld* gw, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head)
	  : Avatar(gw, radius, pos, head)
{
}



void Square::Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	D3DXMATRIX T, R, S;

	// Begin passes.
	UINT numPasses = 0;
	HR(FX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(FX->BeginPass(i));

		D3DXMatrixScaling(&S, Scale().x, Scale().y, Scale().z);
		D3DXMatrixRotationQuaternion(&R, &m_qHeading);

		D3DXMatrixTranslation(&T, Position().x, Position().y, Position().z);

		HR(FX->SetMatrix(mhWVP, &(S*R*T*viewMatrix*projMatrix)));
		HR(FX->CommitChanges());
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 8, 12, 12));

		HR(FX->EndPass());
	}
	HR(FX->End());
}

Sphere::Sphere(GameWorld* gw, ID3DXMesh* sphereMesh, int id) : Avatar(gw), m_mSphere(sphereMesh)
{
	//assert( (( id >= m_iNextID ) || ( id == -1 )) && "Sphere ID must be greater than the ID of the last sphere created");
	if ( id == -1 )
		m_iID = m_iNextID++;
	else
	{
		m_iID = id;
		m_iNextID = id + 1;
	}
}

Sphere::Sphere(GameWorld* gw, ID3DXMesh* sphereMesh, float radius, D3DXVECTOR3 pos, D3DXVECTOR3 head)
	    : Avatar(gw, radius, pos, head), m_mSphere(sphereMesh)
{

}

void Sphere::Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	D3DXMATRIX T, R, S;

	// Begin passes.
	UINT numPasses = 0;
	HR(FX->Begin(&numPasses, 0));
	for(UINT i = 0; i < numPasses; ++i)
	{
		HR(FX->BeginPass(i));

		D3DXMatrixScaling(&S, Scale().x, Scale().y, Scale().z);
		D3DXMatrixRotationQuaternion(&R, &m_qHeading);
		//wout.printD3DXQuat(m_qHeading, "Sphere Quat", 0.1, 0.0005, true);
		D3DXMatrixTranslation(&T, Position().x, Position().y, Position().z);

		HR(FX->SetMatrix(mhWVP, &(S*R*T*viewMatrix*projMatrix)));
		HR(FX->CommitChanges());
		HR(m_mSphere->DrawSubset(0));

		HR(FX->EndPass());
	}
	HR(FX->End());
}

Line::Line(GameWorld* gw, int id, ID3DXLine* lineDrawer) : Avatar(gw), m_pLineDrawer(lineDrawer)
{
	//assert( (( id >= m_iNextID ) || ( id == -1 )) && "Line ID must be greater than the ID of the last Line created");
	if ( id == -1 )
		m_iID = m_iNextID++;
	else
	{
		m_iID = id;
		m_iNextID = id + 1;
	}
}

void Line::Render(ID3DXEffect* FX, D3DXHANDLE mhWVP, D3DXMATRIX viewMatrix, D3DXMATRIX projMatrix)
{
	D3DXMATRIX T = viewMatrix*projMatrix;
	D3DXVECTOR3 points[2];

	points[0] = Position();
	points[1] = m_vEndPosition;

	HR(m_pLineDrawer->DrawTransform(points, 2, &T, Color()));
}
