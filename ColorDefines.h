//ColerDefines.h file
//Created by: Cameron Ferguson - November 6, 2010
//Description: Holds defines for commonly used colors
#ifndef COLOR_DEFINES_H
#define COLOR_DEFINES_H

#define RED		D3DCOLOR_XRGB(255, 0, 0)
#define GREEN	D3DCOLOR_XRGB(0, 255, 0)
#define BLUE	D3DCOLOR_XRGB(0, 0, 255)
#define TEAL	D3DCOLOR_XRGB(0, 128, 128)
#define PURPLE	D3DCOLOR_XRGB(255, 0, 255)
#define	YELLOW	D3DCOLOR_XRGB(255, 255, 0)
#define ORANGE	D3DCOLOR_XRGB(255, 165, 0)
#define DKBROWN	D3DCOLOR_XRGB(102, 51, 0)
#define BROWN	D3DCOLOR_XRGB(152, 101, 50)
#define LTBROWN	D3DCOLOR_XRGB(202, 151, 100)
#define WHITE	D3DCOLOR_XRGB(255, 255, 255)
#define BLACK	D3DCOLOR_XRGB(0, 0, 0)

static inline unsigned __int8 GET_ALPHA(DWORD col)
{
	return (__int8)((col & 0xff000000) >> 24);
}

static inline unsigned __int8 GET_RED(DWORD col)
{
	return (__int8)((col & 0xff0000) >> 16);
}

static inline unsigned __int8 GET_GREEN(DWORD col)
{
	return (__int8)((col & 0xff00) >> 8);
}

static inline unsigned __int8 GET_BLUE(DWORD col)
{
	return (__int8)((col & 0xff));
}

#endif