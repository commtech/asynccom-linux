#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_ */

int main(void)
{
    int fd = 0;
    unsigned status = 0;

    fd = open("/dev/ttyUSB0", O_RDWR);

    ioctl(fd, IOCTL_ASYNCCOM_GET_ECHO_CANCEL, &status);

    ioctl(fd, IOCTL_ASYNCCOM_DISABLE_ECHO_CANCEL);
    ioctl(fd, IOCTL_ASYNCCOM_ENABLE_ECHO_CANCEL);

    close(fd);

    return 0;
}
