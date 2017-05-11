
#include <windows.h>
#include <stdio.h>
#include "Keys.h"

using namespace std;

int main(int argc, char* argv[])
{
	/*
	//Extra code to specify windows
	char* table = "UnityWndClass";
	char* dos = "SDL_app";
	HWND window = ::FindWindowEx(0, 0, table , 0);
	// Bring specified window into focus
	SetForegroundWindow(window);
	*/

	//Calls in Keys.cpp
	setup();
	space();
	left();
	right();
	up();
	down();
	
	aRight();
	space();
	aLeft();

	return 0;
}