# Frame Length

The frame length specifies the number of bytes that get transmitted between the start and stop bits. The standard asynchronous serial communication protocol uses a frame length of one.

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_FRAME_LENGTH
```

###### Examples
```c
#include <asynccom.h>
...

unsigned frame_length;

ioctl(fd, IOCTL_ASYNCCOM_GET_FRAME_LENGTH, &frame_length);
```

## Enable
### IOCTL
```c
IOCTL_ASYNCCOM_ENABLE_FRAME_LENGTH
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |
| `EPROTONOSUPPORT` | 93 (0x5D) | Not supported on this family of cards |

###### Examples
Transmit 4 bytes per frame.
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_ENABLE_FRAME_LENGTH, 4);
```

## Disable
### IOCTL
```c
IOCTL_ASYNCCOM_DISABLE_FRAME_LENGTH
```

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_DISABLE_FRAME_LENGTH);
```


### Additional Resources
- Complete example: [`examples/frame-length.c`](../examples/frame-length.c)
