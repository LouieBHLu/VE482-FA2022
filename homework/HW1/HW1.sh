useradd -m pastrol
ps -le
cat /proc/cpuinfo
cat /proc/meminfo
head -n 5 /dev/random | tee 1 > 2
cat 1 2 > 3
hexdump 3
grep -lR "ddekit_sem_down" /usr/src | grep "semaphore"