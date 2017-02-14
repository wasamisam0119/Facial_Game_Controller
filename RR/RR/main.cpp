#include <cstdio>
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv){
	char input;

	ofstream a_file("input.txt");
	if (!a_file.is_open()) {
		printf("File was not opened\n");
		return 0;
	}
	for (int i = 0; i < 10; i++) {
		input = getchar();
		if (input == '\n') {
			i--;
		}
		else {
			a_file << input;
			a_file << "\n";
		}
		
	}
	a_file.close();
}