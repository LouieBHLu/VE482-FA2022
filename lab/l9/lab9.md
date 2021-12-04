## Tasks

**What needs to be returned by read and write file operations for a character device?**

If succeeds, the value returned by read or write can be: the number of bytes transferred.

If fails, they will return -1.

**How are exactly those major and minor numbers working?**

The major number identifies the driver associated with the device and is a small integer that serves as the index into a static array of char drivers. 

The minor number is used only by the driver specified by the major number and is used only by the driver specified by the major number.

**Knowing the major number and minor numbers of a device, how to add a character device to /dev?**

```C
cdev_add(struct cdev *dev, dev_t num, unsigned int num);
```

**Where are the following terms located in linux source code?**

1. module_init	include/linux/module.h

2. module_exit    include/linux/module.h

3. printk  /include/linux/printk.h

4. container_of    /include/linux/kernel.h

5. dev_t  /include/linux/types.h
6. MAJOR  /include/linux/kdev_t.h
7. MINOR  /include/linux/kdev_t.h
8. MKDEV  /include/linux/kdev_t.h
9. alloc_chrdev_region  /include/linux/fs.h
10. module_param  /include/linux/moduleparam.h
11. cdev_init   /include/linux/cdev.h
12. cdev_add  /include/linux/cdev.h
13. cdev_del  /include/linux/cdev.h
14. THIS_MODULE  /include/linux/export.h

**How to generate random numbers when working inside the Linux kernel?**

`void get_random_bytes(void *buf, int nbytes)`

**How to define and specify module options?**

 Use the MODULE_PARM() macro.

The variable declarations and macros should be placed at the beginning of the module for clarity.

## Result (See /src)

```shell
lxj@ubuntu:~/Documents/lab9$ make
make -C /lib/modules/5.11.0-40-generic/build M=/home/lxj/Documents/lab9 modules
make[1]: Entering directory '/usr/src/linux-headers-5.11.0-40-generic'
  CC [M]  /home/lxj/Documents/lab9/dice.o
  LD [M]  /home/lxj/Documents/lab9/dicedevice.o
  MODPOST /home/lxj/Documents/lab9/Module.symvers
  CC [M]  /home/lxj/Documents/lab9/dicedevice.mod.o
  LD [M]  /home/lxj/Documents/lab9/dicedevice.ko
  BTF [M] /home/lxj/Documents/lab9/dicedevice.ko
Skipping BTF generation for /home/lxj/Documents/lab9/dicedevice.ko due to unavailability of vmlinux
make[1]: Leaving directory '/usr/src/linux-headers-5.11.0-40-generic'
lxj@ubuntu:~/Documents/lab9$ sudo rmmod dicedevice
lxj@ubuntu:~/Documents/lab9$ sudo insmod dicedevice.ko
lxj@ubuntu:~/Documents/lab9$ sudo mknod /dev/dice0 c 42 0
mknod: /dev/dice0: File exists
lxj@ubuntu:~/Documents/lab9$ echo 2 |sudo tee  /dev/dice0
2
lxj@ubuntu:~/Documents/lab9$ cat /dev/dice0
-------  -------  
| o o |  | o o |  
|  o  |  |  o  |  
| o o |  | o o |  
-------  -------  
lxj@ubuntu:~/Documents/lab9$ echo 2 |sudo tee  /dev/dice0
2
lxj@ubuntu:~/Documents/lab9$ cat /dev/dice0
-------  -------  
| o o |  | o o |  
| o o |  |  o  |  
| o o |  | o o |  
```

