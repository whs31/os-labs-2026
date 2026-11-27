#!/bin/bash
# Собирает initramfs (busybox + модуль + тест) и загружает ядро хоста в QEMU
set -e
BUSYBOX=${BUSYBOX:-$HOME/.cache/lab11/busybox}
ROOT=$(mktemp -d)
mkdir -p $ROOT/{bin,dev,proc,sys,tmp,root}
cp $BUSYBOX $ROOT/bin/busybox
for a in $($BUSYBOX --list | grep -vx busybox); do ln -sf busybox $ROOT/bin/$a; done
cp whs31_chardev.ko test $ROOT/root/
cat > $ROOT/init <<'INIT'
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs dev /dev
hostname lab11-vm
dmesg -n 1
cd /root
export HOME=/root PS1='\[\e[1;32m\]whs31@\h\[\e[0m\]:\[\e[1;34m\]\w\[\e[0m\]# '
exec setsid cttyhack sh
INIT
chmod +x $ROOT/init
(cd $ROOT && find . | cpio -o -H newc --quiet | gzip) > initramfs.img
rm -rf $ROOT
exec qemu-system-x86_64 -enable-kvm -m 512M -nographic -no-reboot \
  -kernel /boot/vmlinuz-linux -initrd initramfs.img \
  -append "console=ttyS0 quiet loglevel=3 panic=-1"
