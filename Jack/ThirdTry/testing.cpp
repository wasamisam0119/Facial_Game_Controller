#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
  FILE *fp = fopen("testpipe", "w");
  int i = 0;
  for (; i < 59; i++) {
    fprintf(fp, "%d\n", i);
  }
  fprintf(fp, "000\n");
  fclose(fp);
}
