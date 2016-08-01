#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */

int main(void)
{
    int fd = 0;
    unsigned status = 0;

    fd = open("/dev/ttyUSB0", O_RDWR);

    ioctl(fd, IOCTL_ASYNCCOM_GET_EXTERNAL_TRANSMIT, &status);

    ioctl(fd, IOCTL_ASYNCCOM_DISABLE_EXTERNAL_TRANSMIT);

    /* External transmit every 1 character */
    ioctl(fd, IOCTL_ASYNCCOM_ENABLE_EXTERNAL_TRANSMIT, 1);

    close(fd);

    return 0;
}
