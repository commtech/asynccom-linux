# asynccom-linux
This README file is best viewed [online](http://github.com/commtech/asynccom-linux/).

## Installing Driver

##### Downloading Driver Source Code
The source code for the SerialFC driver is hosted on Github code hosting. To check
out the latest code you will need Git and to run the following in a terminal.

```
git clone git://github.com/commtech/asynccom-linux.git asynccom
```

You can also download driver packages directly from our
[website](http://www.commtech-fastcom.com/CommtechSoftware.html).

##### Switch To Stable Version
Now that you have the latest code checked out, switch to the latest stable
version v2.0.1 is only listed here as an example.

```
git tag
git checkout v2.0.1
```

##### Build Source Code
Run the make command from within the source code directory to build the driver.

```
cd asynccom/
make
```

If you would like to enable debug prints within the driver you need to add
the DEBUG option while building the driver.

```
make DEBUG=1
```

Once debugging is enabled you will find extra kernel prints in the
/var/log/messages and /var/log/debug log files.

If the kernel header files you would like to build against are not in the
default location `/lib/modules/$(shell uname -r)/build` then you can specify
the location with the KDIR option while building the driver.

```
make KDIR="/location/to/kernel_headers/"
```

##### Loading Driver
Assuming the driver has been successfully built in the previous step you are
now ready to load the driver so you can begin using it. To do this you insert
the driver's kernel object file (asynccom.ko) into the kernel.

```
insmod asynccom.ko
```

_You will more than likely need administrator privileges for this and
the following commands._

If no cards are present you will see the following message.

```
insmod asynccom.ko

_All driver load time options can be set in your modprobe.conf file for
using upon system boot_

You can verify that the new ports were detected by checking the message log.

```
tail /var/log/kern.log
```

```
Async Com usb asynchronous serial adapter converter now attached to ttyUSB0
```

##### Installing Driver
If you would like the driver to automatically load at boot use the included
installer.

```
make install
```

This will also install the header (.h) files.

To uninstall, use the included uninstaller.

```
make uninstall
```

## Quick Start Guide
There is documentation for each specific function listed below, but lets get started
with a quick programming example for fun.

_This tutorial has already been set up for you at_
[`asynccom/examples/tutorial.c`](https://github.com/commtech/asynccom-linux/tree/master/examples/tutorial.c).

Create a new C file (named tutorial.c) with the following code.

```c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <asynccom.h>

int main(void)
{
    int fd = 0;
    char odata[] = "Hello world!";
    char idata[20];
    int file_status;
    unsigned type;
    struct termios tios;

    /* Open port 0 (ttyUSB0) */
    fd = open("/dev/ttyUSB0", O_RDWR | O_NDELAY);

    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /* Turn off O_NDELAY now that we have the port open */
    file_status = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, file_status & ~O_NDELAY);

    ioctl(fd, IOCTL_ASYNCCOM_GET_CARD_TYPE, &type);

    /* Configure serial settings, see termios documention for more details */
    tcgetattr(fd, &tios);
    tios.c_iflag = IGNBRK;
    tios.c_oflag = 0;
    tios.c_cflag = CS8 | CREAD | CLOCAL;
    tios.c_lflag &= ~(ICANON | ISIG | ECHO);
    tios.c_cc[VTIME] = 1;   //set the read timeout value in deciseconds
    tios.c_cc[VMIN]  = 20; //set to the number of characters you want to read
    tcsetattr(fd, TCSANOW, &tios);

    /* Send "Hello world!" text */
    write(fd, odata, sizeof(odata));

    /* Read the data back in (with our loopback connector) */
    read(fd, idata, sizeof(idata));

    fprintf(stdout, "%s\n", idata);

    close(fd);

    return EXIT_SUCCESS;
}
```

For this example I will use the gcc compiler, but you can use your compiler of
choice.

```
# gcc -I ../lib/raw/ tutorial.c
```

Now attach the included loopback connector.

```
# ./a.out
Hello world!
```

You have now transmitted and received an asynchronous frame!


## API Reference

There are likely other configuration options you will need to set up for your
own program. All of these options are described on their respective documentation page.

- [Connect](docs/connect.md)
- [Clock Rate](docs/clock-rate.md)
- [Custom Divisor](docs/custom-divisor.md)
- [Echo Cancel](docs/echo-cancel.md)
- [External Transmit](docs/external-transmit.md)
- [Frame Length](docs/frame-length.md)
- [Isochronous](docs/isochronous.md)
- [Read](docs/read.md)
- [RS485](docs/rs485.md)
- [RX Trigger](docs/rx-trigger.md)
- [Sample Rate](docs/sample-rate.md)
- [TX Trigger](docs/tx-trigger.md)
- [Write](docs/write.md)
- [Disconnect](docs/disconnect.md)


### Custom Baud Rates
Setting the baud rate on the Async Com is done by setting three values, clock rate,
sample rate, and a custom divisor. The final baud rate is figured by the following equation:
```
baud rate = clock rate/(sample rate * divisor) 
```
See clock-rate.c, sample-rate.c, and custom-divisor.c for examples on settings these values. 


### FAQ

##### Why am I not receiving all of my data while in RS485 mode?
Manually closing the port immediately after an RS485 transfer could cause the serial
driver to disable the transmitter. Either don't close the port (which will give it time
to clean itself up), run other code, or manually put in a sleep before closing

##### How do I give my user account permissions to touch the serial ports?
```
usermod -a -G dialout <username>
```

##### How do I prevent `setserial` from caching old serial port settings?
In Debian based distributions you can reconfigure the setserial package  and set the default option to `manual`.
```
dpkg-reconfigure setserial
```


## Build Dependencies
- Kernel Build Tools (GCC, make, kernel headers, etc)


## Run-time Dependencies
- OS: Linux
- Base Installation: >= 2.6.16 (might work with a lower version)
- Sysfs Support: >= 2.6.25


## API Compatibility
We follow [Semantic Versioning](http://semver.org/) when creating releases.


## License

Copyright (C) 2016 [Commtech, Inc.](http://fastcomproducts.com)

Licensed under the [GNU General Public License v3](http://www.gnu.org/licenses/gpl.txt).
