#include <Windows.h>
#include "Keys.h"

using namespace std;

INPUT ip;

void setup() {
	//Set up keyboard event
	ip.type = INPUT_KEYBOARD;
	//Scan codes
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
}
void release() {
	//Release key
	Sleep(1000);
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}
void input() {
	SendInput(1, &ip, sizeof(INPUT));
	release();
}
void up() {
	//Press W key
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x57;
	input();
}
void down() {
	//Press S key
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x53;
	input();
}
void left() {
	//Press A key
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x41;
	input();
}
void right() {
	//Press D keys
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x44;
	input();
}
void space() {
	//Press Space keys
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x20;
	input();
}
void aLeft() {
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x26;
	input();
}
void aRight() {
	ip.ki.dwFlags = 0;
	ip.ki.wVk = 0x27;
	input();
}