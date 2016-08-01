#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* IOCTL_ASYNCCOM_* */

int main(void)
{
    int fd = 0;
    unsigned status = 0;
    unsigned type;

    fd = open("/dev/ttyUSB0", O_RDWR | O_NDELAY);
    
    ioctl(fd, IOCTL_ASYNCCOM_GET_RS485, &status);

    /* Enable RS485 */
    ioctl(fd, IOCTL_ASYNCCOM_ENABLE_RS485);

    /* Disable RS485 */
    ioctl(fd, IOCTL_ASYNCCOM_DISABLE_RS485);

    close(fd);
   
    return 0;
}
