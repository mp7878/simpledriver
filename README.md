# simpledriver
This is one of the first drivers that I have made. It uses [IOCTL](https://docs.microsoft.com/en-us/windows/win32/devio/device-input-and-output-control-ioctl-) to transmit data from usermode to kernel mode and vice versa.<br>

# Usage
To use this one must enable [test signing](https://docs.microsoft.com/en-us/windows-hardware/drivers/install/the-testsigning-boot-configuration-option) on their pc because this is a test signed driver.<br> 
You would then load the driver like this in an elevated command prompt: <br>

```
$ sc create MyDriver binPath= path_to_driver type= kernel
$ sc start MyDriver
```

You can now run the example as admin which will print out the current baseaddress of running program.
