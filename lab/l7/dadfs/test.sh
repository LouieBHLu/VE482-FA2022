dd bs=4096 count=100 if=/dev/zero of=disk
dd bs=1M count=10 if=/dev/zero of=journal
mke2fs -b 4096 -O journal_dev journal
./mkfs-dadfs disk
sudo insmod ./dadfs.ko
test_journal_dev=$(sudo losetup -f --show journal)
sudo mount -o loop,owner,group,users,journal_path="$test_journal_dev" -t dadfs disk /mnt