#include <stdio.h>

int main() {

  FILE *fp = fopen("testpipe", "w");
  int i = 0;

  for (; i < 58; i++) {
    fprintf(fp, "%d\n", i);
  };

  fclose(fp);

}
