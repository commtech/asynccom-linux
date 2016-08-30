# Write

###### Code Support
| Code | Version |
| ---- | ------- |
| asynccom-linux | 1.0.0 |


## Write
The Linux [`write`](http://linux.die.net/man/3/write) is used to write data to the port.

###### Examples
### Function
```c
#include <unistd.h>
...

char odata[] = "Hello world!";
unsigned bytes_written;

bytes_written = write(fd, odata, sizeof(odata));
```

### Command Line
###### Examples
```
echo "Hello world!" > /dev/ttyUSB0
```

### Additional Resources
- Complete example: [`examples/tutorial.c`](../examples/tutorial.c)
