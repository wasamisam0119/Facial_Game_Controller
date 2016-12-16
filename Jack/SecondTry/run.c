#include <stdio.h>
#include <string.h>

int main() {
    char input;
    int count = 0;

    while(1) {
        input = scanf("%c", &input);
        printf("Count: %d, Input: %c", count, input);
        /*
        if(strncmp(&input, "\n", 1) == 0) {
        } else if(strncmp(&input, "h", 1) ==  0) {
            printf("h? Try harder.\tCount = %d\n", count++);
        } else {
            printf("That's better.\tCount = %d\n", count++);
        }
        */
    }
    return 0;
}
