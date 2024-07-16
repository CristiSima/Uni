cd output/images
sudo losetup -P /dev/loop0 rootfs.img
sudo mount /dev/loop0p2  rootfs_p2

cd rootfs_p2
sudo tar -x -f ../rootfs.tar

cd ..
sudo umount rootfs_p2
sudo losetup -d /dev/loop0
