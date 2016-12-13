#include <stdio.h>
#include <string.h>

int main() {
    char input;
    int count = 0;
    while(1) {
        input = getchar();
        if(strncmp(&input, "h",  1)) {
            printf("H? Try harder.\n");
        } else {
            printf("Better.\n");
        }
        count++;
        printf("Counter: %d\n", count);
    }
    return 0;
}
