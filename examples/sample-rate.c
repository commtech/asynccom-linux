#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int fd = 0;
    unsigned rate;
    int samplerate;

    fd = open("/dev/ttyUSB0", O_RDWR);
    
    samplerate = atoi(argv[1]);

    ioctl(fd, IOCTL_ASYNCCOM_GET_SAMPLE_RATE, &rate);

    ioctl(fd, IOCTL_ASYNCCOM_SET_SAMPLE_RATE, samplerate);

    close(fd);

    return 0;
}
