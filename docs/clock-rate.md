# Clock Rate

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |




###### Operating Range
| Card Family | Range |
| ----------- | ----- |
| Async Com (16C950) | 200 Hz - 270 MHz |



## Set
### IOCTL
```c
IOCTL_ASYNCCOM_SET_CLOCK_RATE
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |


###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_SET_CLOCK_RATE, 32);
```

### Additional Resources
- Complete example: [`examples/clock-rate.c`](../examples/clock-rate.c)
