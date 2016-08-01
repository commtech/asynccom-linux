# Custom Divisor

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |



## Set
### IOCTL
```c
IOCTL_ASYNCCOM_SET_DIVISOR
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |


###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_SET_DIVISOR, 2);
```

### Additional Resources
- Complete example: [`examples/custom-divisor.c`](../examples/custom-divisor.c)
