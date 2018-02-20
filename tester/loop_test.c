#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <asynccom.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd = 0;
    char odata[] = "Hello world!";
    char idata[20];
    int file_status;
    unsigned type;
    struct termios tios;
    int i;

    if(argc < 2){
	printf("usage: ./loop_test num_loops\n");
    }

    /* Open port 0 (ttyUSB0) */
    fd = open("/dev/ttyUSB0", O_RDWR | O_NDELAY);

    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /* Turn off O_NDELAY now that we have the port open */
    file_status = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, file_status & ~O_NDELAY);


    /* Configure serial settings, see termios documention for more details */
    tcgetattr(fd, &tios);
    tios.c_iflag = IGNBRK;
    tios.c_oflag = 0;
    tios.c_cflag = CS8 | CREAD | CLOCAL;
    tios.c_lflag &= ~(ICANON | ISIG | ECHO);
    tios.c_cc[VTIME] = 1;   //set the read timeout value in deciseconds
    tios.c_cc[VMIN]  = 20; //set to the number of characters you want to read
    tcsetattr(fd, TCSANOW, &tios);

    for(i=0; i<(atoi(argv[1])); i++){
    /* Send "Hello world!" text */
    write(fd, odata, sizeof(odata));

    /* Read the data back in (with our loopback connector) */
    read(fd, idata, sizeof(idata));

    fprintf(stdout, "%d %s\n", i, idata);
     }
    close(fd);

    return EXIT_SUCCESS;
}
