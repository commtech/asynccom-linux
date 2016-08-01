#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */

int main(void)
{
    int fd = 0;
    unsigned length;

    fd = open("/dev/ttyUSB0", O_RDWR);

    ioctl(fd, IOCTL_ASYNCCOM_GET_FRAME_LENGTH, &length);

    ioctl(fd, IOCTL_ASYNCCOM_SET_FRAME_LENGTH, 1);

    close(fd);

    return 0;
}
