#include <stdio.h>

int main() {
    char input;
    FILE *pipe;
    pipe = fopen("pipe", "w");

    while(1) {
        input = getchar();
        fprintf(pipe, "%c", input);
        printf("%c\n", input);
    }
    return 0;
}
