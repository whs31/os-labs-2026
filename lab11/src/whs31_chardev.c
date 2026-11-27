// SPDX-License-Identifier: GPL-2.0
/*
 * whs31_chardev - простой драйвер символьного устройства.
 * Хранит данные в буфере ядра (до 4 КиБ): write пишет, read читает,
 * lseek перемещает позицию, ioctl очищает буфер или возвращает его размер.
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include "whs31_chardev.h"

#define DEV_NAME "whs31_chardev"
#define BUF_SIZE 4096

static dev_t devno;
static struct cdev whs_cdev;
static struct class *whs_class;

static char buffer[BUF_SIZE];
static size_t data_size;
static DEFINE_MUTEX(lock);
static int open_count;

static int whs_open(struct inode *inode, struct file *f)
{
	mutex_lock(&lock);
	open_count++;
	pr_info(DEV_NAME ": open (pid %d, %s), открыто раз: %d\n",
		current->pid, current->comm, open_count);
	mutex_unlock(&lock);
	return 0;
}

static int whs_release(struct inode *inode, struct file *f)
{
	pr_info(DEV_NAME ": release (pid %d)\n", current->pid);
	return 0;
}

static ssize_t whs_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	ssize_t ret = 0;

	mutex_lock(&lock);
	if (*off >= data_size)
		goto out;
	if (len > data_size - *off)
		len = data_size - *off;
	if (copy_to_user(buf, buffer + *off, len)) {
		ret = -EFAULT;
		goto out;
	}
	*off += len;
	ret = len;
	pr_info(DEV_NAME ": read %zu байт, позиция %lld\n", len, *off);
out:
	mutex_unlock(&lock);
	return ret;
}

static ssize_t whs_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	ssize_t ret;

	mutex_lock(&lock);
	if (f->f_flags & O_APPEND)
		*off = data_size;
	if (*off >= BUF_SIZE) {
		ret = -ENOSPC;
		goto out;
	}
	if (len > BUF_SIZE - *off)
		len = BUF_SIZE - *off;
	if (copy_from_user(buffer + *off, buf, len)) {
		ret = -EFAULT;
		goto out;
	}
	*off += len;
	if (*off > data_size)
		data_size = *off;
	ret = len;
	pr_info(DEV_NAME ": write %zu байт, размер данных %zu\n", len, data_size);
out:
	mutex_unlock(&lock);
	return ret;
}

static loff_t whs_llseek(struct file *f, loff_t off, int whence)
{
	loff_t pos;

	mutex_lock(&lock);
	switch (whence) {
	case SEEK_SET: pos = off; break;
	case SEEK_CUR: pos = f->f_pos + off; break;
	case SEEK_END: pos = data_size + off; break;
	default: pos = -EINVAL;
	}
	if (pos >= 0 && pos <= BUF_SIZE)
		f->f_pos = pos;
	else if (pos >= 0)
		pos = -EINVAL;
	mutex_unlock(&lock);
	return pos;
}

static long whs_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	int size;

	mutex_lock(&lock);
	switch (cmd) {
	case WHS_IOC_CLEAR:
		memset(buffer, 0, BUF_SIZE);
		data_size = 0;
		pr_info(DEV_NAME ": ioctl CLEAR\n");
		break;
	case WHS_IOC_GETSIZE:
		size = data_size;
		if (copy_to_user((int __user *)arg, &size, sizeof(size)))
			ret = -EFAULT;
		break;
	default:
		ret = -ENOTTY;
	}
	mutex_unlock(&lock);
	return ret;
}

static const struct file_operations whs_fops = {
	.owner          = THIS_MODULE,
	.open           = whs_open,
	.release        = whs_release,
	.read           = whs_read,
	.write          = whs_write,
	.llseek         = whs_llseek,
	.unlocked_ioctl = whs_ioctl,
};

static int __init whs_init(void)
{
	int ret;
	struct device *dev;

	ret = alloc_chrdev_region(&devno, 0, 1, DEV_NAME);
	if (ret)
		return ret;

	cdev_init(&whs_cdev, &whs_fops);
	ret = cdev_add(&whs_cdev, devno, 1);
	if (ret)
		goto err_region;

	whs_class = class_create("whs31");
	if (IS_ERR(whs_class)) {
		ret = PTR_ERR(whs_class);
		goto err_cdev;
	}
	dev = device_create(whs_class, NULL, devno, NULL, DEV_NAME);
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto err_class;
	}
	pr_info(DEV_NAME ": загружен, major=%d minor=%d\n", MAJOR(devno), MINOR(devno));
	return 0;

err_class:
	class_destroy(whs_class);
err_cdev:
	cdev_del(&whs_cdev);
err_region:
	unregister_chrdev_region(devno, 1);
	return ret;
}

static void __exit whs_exit(void)
{
	device_destroy(whs_class, devno);
	class_destroy(whs_class);
	cdev_del(&whs_cdev);
	unregister_chrdev_region(devno, 1);
	pr_info(DEV_NAME ": выгружен\n");
}

module_init(whs_init);
module_exit(whs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("whs31 (Рязанцев Д.Л.)");
MODULE_DESCRIPTION("Простой драйвер символьного устройства");
