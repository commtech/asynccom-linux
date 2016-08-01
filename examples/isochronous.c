#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */

int main(void)
{
    int fd = 0;
    unsigned status = 0;

    fd = open("/dev/ttyUSB0", O_RDWR);

    ioctl(fd, IOCTL_ASYNCCOM_GET_ISOCHRONOUS, &status);

    ioctl(fd, IOCTL_ASYNCCOM_DISABLE_ISOCHRONOUS);

    /* Enable transmit using external RI# */
    ioctl(fd, IOCTL_ASYNCCOM_ENABLE_ISOCHRONOUS, 0);

    close(fd);

    return 0;
}
