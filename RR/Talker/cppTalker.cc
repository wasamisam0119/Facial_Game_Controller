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

void shell(){
	int sent = 0;
	system("./shell.sh&");	
	for(int i = 0; i < 9; i++){
		if(i%3 == 0){
			system("echo 001 > testpipe&");
			sent++;
		}
		if(i%3 == 1){
			system("echo 002 > testpipe&");
			sent++;
		}
		if(i%3 == 2){
			system("echo 003 > testpipe&");
			sent++;
		}
	}
	system("echo 000 > testpipe");
	sent++;
	printf("Sent: %d\n", sent);
}

void cqueue(){
	
	queue<int> q;
	q.push(100);
	q.push(200);
	q.push(300);
	q.push(400);

	cout << "Size of the queue: " << q.size()
<< endl;

	cout << q.front() << endl;
	q.pop();

	cout << q.front() << endl;
	q.pop();
	
	cout << q.front() << endl;
	q.pop();

	cout << q.front() << endl;
	q.pop();

	if(q.empty()){
		cout << "Queue is empty" << endl;
	}
	
}

int main(){
	
	std::thread first (shell);
	std::thread second (cqueue);
	
	first.join();
	second.join();

	return 0;
}
