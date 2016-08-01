# Echo Cancel

The echo cancel feature disables the receiver while transmitting. This is mainly used in RS485 mode when the transmit and receive lines are tied together.


###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_ECHO_CANCEL
```

###### Examples
```c
#include <asynccom.h>
...

unsigned status;

ioctl(fd, IOCTL_ASYNCCOM_GET_ECHO_CANCEL, &status);
```

## Enable
### IOCTL
```c
IOCTL_ASYNCCOM_ENABLE_ECHO_CANCEL
```

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_ENABLE_ECHO_CANCEL);
```

## Disable
### IOCTL
```c
IOCTL_ASYNCCOM_DISABLE_ECHO_CANCEL
```

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_DISABLE_ECHO_CANCEL);
```

### Additional Resources
- Complete example: [`examples/echo-cancel.c`](../examples/echo-cancel.c)
