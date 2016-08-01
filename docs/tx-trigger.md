# TX Trigger

The TX FIFO trigger level generates an interrupt whenever the data level in the transmit FIFO falls below this preset trigger level.

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |

###### Operating Range
| Card Family | Range |
| ----------- | ----- |
| Async Com (16C950) | 0 - 127 |


## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_TX_TRIGGER
```

###### Examples
```c
#include <asynccom.h>
...

unsigned level;

ioctl(fd, IOCTL_ASYNCCOM_GET_TX_TRIGGER, &level);
```

## Set
### IOCTL
```c
IOCTL_ASYNCCOM_SET_TX_TRIGGER
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_SET_TX_TRIGGER, 32);
```

### Additional Resources
- Complete example: [`examples/tx-trigger.c`](../examples/tx-trigger.c)
