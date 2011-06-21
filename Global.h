#ifndef GLOBAL_H
#define GLOBAL_H

//Global Class - Will contain all globally accessed variables and
//commonly used functions

#include <vector>
#include "Graphics\gspOut.h"
#include "Algo\Edge.h"

using namespace std;

#define GLB Global::Instance()

class Global
{
private:
	Global(){}

	//Time variables
	//current time
	double current;
	double elapsed;

	//Tracker for num objects
	int counter;

public:
	static Global* Instance()
	{
		static Global temp;
		return &temp;
	}
	//Global Gfx Stuff
	//Circle IDs
	int circID;
	//Line IDs
	int linID;

	//Global A* stuffs
	//Start point for A*
	int as_start;
	//End point for A*
	int as_end;
	//Distance travelled
	int as_travelled;
	//Path travelled;
	vector<int> as_final;

	//getters
	double GetTime()
	{
		return current;
	}
	
	int GetCount()
	{
		return counter;
	}

	//setters
	void SetTime(double elap, double cur)
	{
		elapsed = elap;
		current = cur;
	}
	void SetCount(int cnt)
	{
		counter = cnt;
	}
	void SetIds()
	{
		circID = 0;
		linID = 0;
	}

};

#endif