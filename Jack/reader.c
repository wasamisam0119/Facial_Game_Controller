#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define MAX_BUF 1024

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];

    /* open, read, and display the message from the FIFO */
    while(1){
        time_t timeBase = time(NULL);
        char *currTimeHere = ctime(&timeBase);  // Find the current time for this program

        fd = open(myfifo, O_RDONLY);
        read(fd, buf, MAX_BUF);     // Read from the FIFO

        // Output recieved time (that at the sending program) and the local time (that here)
        printf("Received %s at %s local time\n", buf, currTimeHere);    
        close(fd);
    }

    return 0;
}
