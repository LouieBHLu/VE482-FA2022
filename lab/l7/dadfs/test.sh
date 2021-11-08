dd bs=4096 count=100 if=/dev/zero of=disk
dd bs=1M count=10 if=/dev/zero of=journal
mke2fs -b 4096 -O journal_dev journal
./mkfs-dadfs disk
sudo insmod ./dadfs.ko
sudo losetup --find --show disk
lsblk --fs
sudo mount /dev/loop11 /mnt/
# # --------------
dd bs=4096 count=100 if=/dev/zero of=disk
dd bs=1M count=10 if=/dev/zero of=journal
mke2fs -b 4096 -O journal_dev journal
test_journal_dev=$(losetup -f --show journal)
mount -o loop,owner,group,users,journal_path="$test_journal_dev" -t dadfs disk /mnt