#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
  system("./xdostuff.sh&");
  FILE *fp = fopen("testpipe", "w");
  int i, charin;
  while(1) {
    charin = getchar();
    printf("%c\n", charin);
    switch (charin) {
      case 'q': fprintf(fp, "024");
                break;
      case 'w': fprintf(fp, "025");
                break;
      case 'e': fprintf(fp, "026");
                break;
      case 'r': fprintf(fp, "027");
                break;
      case 't': fprintf(fp, "028");
                break;
      case 'y': fprintf(fp, "029");
                break;
      case 'u': fprintf(fp, "030");
                break;
      case 'i': fprintf(fp, "031");
                break;
      case 'o': fprintf(fp, "032");
                break;
      case 'p': fprintf(fp, "033");
                break;
      case 'a': fprintf(fp, "038");
                break;
      case 's': fprintf(fp, "039");
                break;
      case 'd': fprintf(fp, "040");
                break;
      case 'f': fprintf(fp, "041");
                break;
      case 'g': fprintf(fp, "042");
                break;
      case 'h': fprintf(fp, "043");
                break;
      case 'j': fprintf(fp, "044");
                break;
      case 'k': fprintf(fp, "045");
                break;
      case 'l': fprintf(fp, "046");
                break;
      case 'z': fprintf(fp, "052");
                break;
      case 'x': fprintf(fp, "053");
                break;
      case 'c': fprintf(fp, "054");
                break;
      case 'v': fprintf(fp, "055");
                break;
      case 'b': fprintf(fp, "056");
                break;
      case 'n': fprintf(fp, "057");
                break;
      case 'm': fprintf(fp, "058");
                break;
      case '0': fprintf(fp, "000"); fclose(fp); break;
    }
  }
  //for (i = 0; i < 59; i++) {
  //fprintf(fp, "%d\n", i);
  //}
  fprintf(fp, "000");
  fclose(fp);
}
