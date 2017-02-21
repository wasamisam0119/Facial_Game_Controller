#include <cstdio>
#include <iostream>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
#include <mutex>

// Use g++ -std=c++11 test.cc -lpthread -o run

using namespace std;
std::mutex mtx;

int in;
int hold = 0;

void shell(){
	int sent = 0;
	
	system("./shell.sh&");
	while(1){
		mtx.try_lock();
		if(in == 'q') break;
		if(hold != 0) hold--;
		switch(in){
		case 'a': system("echo 001 > testpipe");
					sent++;
					break;
		case 'b': system("echo 002 > testpipe");
					sent++;
					break;
		case 'c': system("echo 003 > testpipe");
					sent++;
					break;
		default: break;
		}
		mtx.unlock();
	}
	system("echo 000 > testpipe");
			sent++;
	printf("Sent: %d\n", sent);
}

void detector(){
	while(1){
		if(!hold){
			mtx.try_lock();
			in = getchar();
			hold++;
		}
		mtx.unlock();
		if(in == 'q') break;
	}
	printf("End of detector\n");
}

int main(){	
	std::thread first (shell);
	std::thread second (detector);
	
	first.join();
	printf("First join\n");
	second.join();
	printf("Second Join\n");
	return 0;
}
