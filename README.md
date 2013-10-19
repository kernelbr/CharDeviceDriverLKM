CharDeviceDriverLKM
===================

An example of Linux Char Device Driver

```
# mknod /dev/test c 66 1
# chmod a+r+w /dev/test
$ echo -n 7 > /dev/test
$ cat /dev/test
7
```
