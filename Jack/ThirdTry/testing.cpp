#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
  system("./xdostuff.sh&");
  FILE *fp = fopen("testpipe", "w");
  int i = 0;
  //for (; i < 59; i++) {
    //fprintf(fp, "%d\n", i);
  //}
  while(i != 0) {
    int in = getchar();
    if (in == 'a') {
      i = 22;
    } else if (in == 'b') {
      i = 23;
    } else if (in == 'c') {
      i = 24;
    } else {
      in = 0;
    }
    fprintf(fp, "%d\n", i);
  }
  fclose(fp);
}
