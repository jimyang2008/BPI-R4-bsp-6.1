#!/bin/bash

set -e

[ -s "./env.sh" ] || die "please run ./configure first."
. ./env.sh

MNTDIR=/mnt/rootfs
ROOTFS_TGZ=${TOPDIR}/rootfs.tar.gz
kernel=6.6.104

BOOT_MEDIA=${1:-''}
BUILD_MODE=${2:-''}
test -n "$BOOT_MEDIA" || select BOOT_MEDIA in sdmmc emmc
do
  test -n "$BOOT_MEDIA" && break
done

# update uEnv.txt and copy boot files
sed -i -e "s/^fdt=.*/fdt=mt7988a-bananapi-bpi-r4-${BOOT_MEDIA}-snand.dtb/" $TOPDIR/mt-pack/mtk/bpi-r4/configs/default/${service}/uEnv.txt
test -n "$BUILD_MODE" && ( cd $TOPDIR; ./build.sh $BUILD_MODE )


IMG_FILE=${BOARD}-linux-${kernel}-${BOOT_MEDIA}.img
rm -f $IMG_FILE


# prepare image file
truncate -s 8G $IMG_FILE
LOOP_DEV=`sudo losetup -fP --show ${IMG_FILE}`
trap "sudo umount -R $MNTDIR; sudo losetup -d $LOOP_DEV" TERM EXIT

# drop-in bootloader/u-boot
GPT=$TOPDIR/mt-pack/mtk/bpi-r4/bin/gpt
BL2=$TOPDIR/atf-mt/build_${BOOT_MEDIA}/mt7988/release/bl2.img
FIP=$TOPDIR/u-boot-mt/build_${BOOT_MEDIA}/u-boot_${BOOT_MEDIA}.fip

sudo dd if=$GPT	of=${LOOP_DEV} conv=fsync,notrunc
sudo dd if=$BL2	of=${LOOP_DEV} conv=fsync,notrunc bs=512 seek=34
sudo dd if=$FIP	of=${LOOP_DEV} conv=fsync,notrunc bs=512 seek=13312

#sudo sgdisk -e $LOOP_DEV
sudo partprobe $LOOP_DEV ||true

# prepare file system
sudo mkfs.fat -F 32 -n BPI-BOOT ${LOOP_DEV}p5
sudo mkfs.ext4 -L BPI-ROOT ${LOOP_DEV}p6

# drop-in BPI-ROOT: rootfs and kernel modules
sudo mount ${LOOP_DEV}p6 $MNTDIR
sudo tar -xzf $ROOTFS_TGZ -C $MNTDIR
sudo rsync -rlptKHW $TOPDIR/SD/BPI-ROOT/ $MNTDIR/

# drop-in BPI-BOOT: kernel image, DTB and uEnv.txt
sudo mount ${LOOP_DEV}p5 $MNTDIR/boot
sudo rsync -r $TOPDIR/SD/BPI-BOOT/* $MNTDIR/boot/

${DEBUG:-false} && read -p 'press ENTER to continue' x

echo "Image $IMG_FILE created successfully"
