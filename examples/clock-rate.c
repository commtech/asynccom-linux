#include <fcntl.h> /* open, O_RDWR */
#include <unistd.h> /* close */
#include <asynccom.h> /* asynccom IOCTLs */
#include "calculate-clock-bits.h"

int main(void)
{
    int fd = 0;
    unsigned char clock_bits[20];
    
    fd = open("/dev/ttyUSB0", O_RDWR);

    calculate_clock_bits(18432000, 10, clock_bits);
    ioctl(fd, IOCTL_ASYNCCOM_SET_CLOCK_BITS, &clock_bits);
    close(fd);

    return 0;
}
