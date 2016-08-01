# Sample Rate

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


###### Operating Range
| Card Family | Range |
| ----------- | ----- |
| Async Com (16C950) | 4 - 16 |



## Get
### IOCTL
```c
IOCTL_ASYNCCOM_GET_SAMPLE_RATE
```

###### Examples
```c
#include <asynccom.h>
...

unsigned rate;

ioctl(fd, IOCTL_ASYNCCOM_GET_SAMPLE_RATE, &rate);
```


## Set
### IOCTL
```c
IOCTL_ASYNCCOM_SET_SAMPLE_RATE
```

| System Error | Value | Cause |
| ------------ | -----:| ----- |
| `EINVAL` | 22 (0x16) | Invalid parameter |

###### Examples
```c
#include <asynccom.h>
...

ioctl(fd, IOCTL_ASYNCCOM_SET_SAMPLE_RATE, 32);
```

### Additional Resources
- Complete example: [`examples/sample-rate.c`](../examples/sample-rate.c)
