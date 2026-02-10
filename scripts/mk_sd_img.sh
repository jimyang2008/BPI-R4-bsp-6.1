#!/bin/bash

set -e

[ -s "./env.sh" ] || die "please run ./configure first."
. ./env.sh

MNTDIR=/mnt/rootfs
ROOTFS_TGZ=${TOPDIR}/rootfs.tar.gz

IMG_FILE=${BOARD}-${service}-sd.img
rm -f $IMG_FILE

# prepare image file
truncate -s 8G $IMG_FILE
LOOP_DEV=`sudo losetup -fP --show ${IMG_FILE}`

# drop-in bootloader/u-boot
GPT=$TOPDIR/mt-pack/mtk/bpi-r4/bin/gpt
BL2=$TOPDIR/atf-mt/build_sdmmc/mt7988/release/bl2.img
FIP=$TOPDIR/u-boot-mt/build_sdmmc/u-boot_sdmmc.fip

sudo dd if=$GPT	of=${LOOP_DEV} conv=fsync,notrunc
sudo dd if=$BL2	of=${LOOP_DEV} conv=fsync,notrunc bs=512 seek=34
sudo dd if=$FIP	of=${LOOP_DEV} conv=fsync,notrunc bs=512 seek=13312

sudo partprobe $LOOP_DEV

# prepare file system
sudo mkfs.fat -F 32 -n BPI-BOOT ${LOOP_DEV}p5
sudo mkfs.ext4 -L BPI-ROOT ${LOOP_DEV}p6

sudo losetup -d ${LOOP_DEV}

# drop-in BPI-ROOT: rootfs and kernel modules
sudo mount ${LOOP_DEV}p6 $MNTDIR
sudo tar -xzf $ROOTFS_TGZ -C $MNTDIR
sudo rsync -rlptKHW $TOPDIR/SD/BPI-ROOT/ $MNTDIR/

# drop-in BPI-BOOT: kernel image, DTB and uEnv.txt
sudo mount ${LOOP_DEV}p5 $MNTDIR/boot
sudo rsync -r $TOPDIR/SD/BPI-BOOT/* $MNTDIR/boot/

read -p 'press ENTER to continue' x
# clean
sudo umount -R $MNTDIR
sudo losetup -d $LOOP_DEV
