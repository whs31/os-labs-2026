#ifndef WHS31_CHARDEV_H
#define WHS31_CHARDEV_H
#include <linux/ioctl.h>

#define WHS_IOC_MAGIC   'w'
#define WHS_IOC_CLEAR   _IO(WHS_IOC_MAGIC, 0)
#define WHS_IOC_GETSIZE _IOR(WHS_IOC_MAGIC, 1, int)

#endif
