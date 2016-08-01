# Connect

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


## Connect
The Linux [`open`](http://linux.die.net/man/3/open) is used to connect to the port.

###### Examples
Connect to port 0.
```c
#include <fcntl.h>
...

int fd;

fd = open("/dev/ttyUSB0", O_RDWR);
```


### Additional Resources
- Complete example: [`examples/tutorial.c`](../examples/tutorial.c)
