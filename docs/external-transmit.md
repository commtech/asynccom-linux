# External Transmit

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |



###### Operating Range
| Card Family | Range |
| ----------- | ----- |
| Async Com (16C950) | 0 - 8191 |


## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_EXTERNAL_TRANSMIT
```

###### Examples
```c
#include <asynccom.h>
...

unsigned status;

ioctl(fd, IOCTL_ASYNCCOM_GET_EXTERNAL_TRANSMIT, &status);
```

## Enable
### IOCTL
```c
IOCTL_ASYNCCOM_ENABLE_EXTERNAL_TRANSMIT
```

###### Examples
Enable external transmit on 4 bytes.
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_ENABLE_EXTERNAL_TRANSMIT, 4);
```

## Disable
### IOCTL
```c
IOCTL_ASYNCCOM_DISABLE_EXTERNAL_TRANSMIT
```

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_DISABLE_EXTERNAL_TRANSMIT);
```

### Additional Resources
- Complete example: [`examples/external-transmit.c`](../examples/external-transmit.c)
