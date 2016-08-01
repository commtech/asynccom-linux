#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */

int main(void)
{
    int fd = 0;
    unsigned level;

    fd = open("/dev/ttyUSB0", O_RDWR);

    ioctl(fd, IOCTL_ASYNCCOM_GET_RX_TRIGGER, &level);

    ioctl(fd, IOCTL_ASYNCCOM_SET_RX_TRIGGER, 32);

    close(fd);

    return 0;
}
