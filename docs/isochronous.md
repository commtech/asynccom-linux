# Isochronous

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


| Mode | Description |
| ----:| ----------- |
| 0 | Transmit using external RI# |
| 1 | Transmit using internal BRG |
| 2 | Receive using external DSR# |
| 3 | Transmit using external RI#, receive using external DSR# |
| 4 | Transmit using internal BRG, receive using external DSR# |
| 5 | Receive using internal BRG |
| 6 | Transmit using external RI#, receive using internal BRG |
| 7 | Transmit using internal BRG, receive using internal BRG |
| 8 | Transmit and receive using external RI# |
| 9 | Transmit clock is output on DTR# |
| 10 | Transmit clock is output on DTR#, receive using external DSR# |


## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_ISOCHRONOUS
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EPROTONOSUPPORT` | 93 (0x5D) | Not supported on this family of cards |

###### Examples
```c
#include <asynccom.h>
...

unsigned status;

ioctl(fd, IOCTL_ASYNCCOM_GET_ISOCHRONOUS, &status);
```

## Enable
### IOCTL
```c
IOCTL_ASYNCCOM_ENABLE_ISOCHRONOUS
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |
| `EPROTONOSUPPORT` | 93 (0x5D) | Not supported on this family of cards |

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_ENABLE_ISOCHRONOUS);
```

## Disable
### IOCTL
```c
IOCTL_ASYNCCOM_DISABLE_ISOCHRONOUS
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EPROTONOSUPPORT` | 93 (0x5D) | Not supported on this family of cards |

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_DISABLE_ISOCHRONOUS);
```

### Additional Resources
- Complete example: [`examples/isochronous.c`](../examples/isochronous.c)
