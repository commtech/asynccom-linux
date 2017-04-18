#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* asynccom IOCTLs */
#include "calculate-clock-bits.h"

int main(void)
{
    int fd = 0;
    int divisor = 2;    

    fd = open("/dev/ttyUSB0", O_RDWR);
	
    ioctl(fd, IOCTL_ASYNCCOM_SET_DIVISOR, divisor);
    close(fd);

    return 0;
}
