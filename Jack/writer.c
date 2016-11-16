#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define MAX_BUF 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";


    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write "Hi" to the FIFO */
    while(1) {
        time_t timeBase = time(NULL);
        char *currTime = ctime(&timeBase);  // 

        fd = open(myfifo, O_WRONLY);
        write(fd, currTime, MAX_BUF);
        close(fd);
        printf("%s\n", currTime);
        sleep(1);
    }
    /* remove the FIFO */
    unlink(myfifo);

    return 0;
}
