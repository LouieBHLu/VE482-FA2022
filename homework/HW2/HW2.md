<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Homework 2</b> - Fall 2021</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Mingxuan Lu<br>ID: 518021911166<br>Email: mingxuan.lu@sjtu.edu.cn<br></b></div>
    </div>
</div>

## Multiprogramming

1. **Probability**: $p^n$； 

   **CPU utilization**: $1-p^n$;

2. <img src="C:\Users\Lenovo\Desktop\VE482\homework\HW2\images\image-20201002215645041.png" alt="image-20201002215645041" style="zoom:50%;" />

3. 

a) Three.

b) 0.271.

c) If add 256MB, the CPU utilization is 0.570 and increase 0.117%

​	if add 512MB, the CPU utilization is 0.771 and increase 0.098%

​	if add 1024MB, the CPU utilization is 0.920 and increase 0.063%

​	Therefore, we can say that adding 256MB is the best.

## Minix 3 Keymap

In `/usr/src/servers/is/dmp.c`, we add:

```c
{SF7, processCnt, "Count Running processes"},
```

into the struct hook_entry, instant hooks.

In `/usr/src/servers/is/dmp_kernel.c`, we add:

```c
#include "../pm/mproc.h"

void processCnt(){
    struct mproc *mp;
    int i,cnt=0;	
    
	getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc);

    for (i = 0; i<NR_PROCS; i++) {
        mp = &mprocs[i];
        if (mp->mp_pid == 0 && i != PM_PROCS_NR) continue;
        cnt++;   
    }
  	printf("Current number of running processes: %d\n",cnt);
}
```

In `/usr/src/servers/is/proto.h`, we add:

```c
void processCnt(void);
```

Then we run the following command to take effect.

```shell
cd /usr/src
make build
sudo reboot
```