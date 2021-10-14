<div style="width:60%;height:200px;text-align:center;border:14px solid #808080;border-top:none;border-left:none;border-bottom:none;display:inline-block">
    <div style="border:4px solid #808080;border-radius:8px;width:95%;height:100%;background-color: rgb(209, 209, 209);">
        <div style="width:100%;height:30%;text-align:center;line-height:60px;font-size:26px;font-family:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;">VE482</div>
        <div style="width:100%;height:18%;text-align:center;line-height:26px;font-size:20px;font-familny:'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;"><b>Homework 1</b> - Fall 2021</div>
        <div style="width:100%;height:57%;text-align:center;font-size:16px;line-height:22px;font-family: 'Courier New', Courier, monospace;font-weight:300;"><br><b>Name: Mingxuan Lu<br>ID: 518021911166<br>Email: mingxuan.lu@sjtu.edu.cn<br></b></div>
    </div>
</div>


## Ex. 1 - Revisions

Stack allocation and heap allocation are two ways of allocating memory in the computer. 

- A stack is a special area of computer’s memory which stores temporary variables created by a function. In stack, variables are declared, stored and initialized during runtime. It is a temporary storage memory. When the computing task is complete, the memory of the variable will be automatically erased. The stack section mostly contains methods, local variable, and reference variables.

- The heap is a memory used by programming languages to store global variables. By default, all global variable are stored in heap memory space. It supports Dynamic memory allocation. The heap is not managed automatically for you and is not as tightly managed by the CPU. It is more like a free-floating region of memory.

## Ex. 2 - Person research

1. BIOS is launched first when the computer is powered on, mainly to ensure the OS is working correctly. BIOS identifies, configures, tests and connects computer hardware to the OS immediately after a computer is turned on. The combination of these steps is called the *boot process*.

   These tasks are each carried out by BIOS' four main functions:

   1. **Power-on self-test (POST)**. This tests the hardware of the computer before loading the OS.
   2. **Bootstrap loader**. This locates the OS.
   3. **Software/drivers.** This locates the software and drivers that interface with the OS once running.
   4. **Complementary metal-oxide semiconductor (CMOS) setup.** This is a configuration program that enable users to alter hardware and system settings. CMOS is the name of BIOS' non-volatile memory.

2. **Hybrid kernel** is a combinational design of microkernel and monolithic kernel that is both stable and efficient. It maintains device drives, which makes it more user-friendly. 

   **Exo kernel** is a design of operating system architecture that separate resource protection from its management so as to take advantage of hardware resources more efficiently and reduce the pressure of testing new functions of the operating system.

## Ex. 3 - Course application

1. a, c, d should be allowed. 

   **Kernel mode**, also known as system mode, is one of the central processing unit (CPU) operating modes. While processes run in kernel mode, they have unrestricted access to the hardware.

   **The other mode** is user mode, which is a non-privileged mode for user programs. Therefore, when a process runs in user mode, it has limited access to the CPU and the memory.

   Therefore, a, c, d should only be restricted in kernel mode. On the other hand, reading the time-of-day clock should be read-only and does nothing to the operating system.

2. When P0 and P1 is assigned to CPU1 and P2 is assigned to CPU2, the total time is 20ms.

   When P0 and P2 is assigned to CPU1 and P1 is assigned to CPU2, the total time is 25ms.

   When P1 and P2 is assigned to CPU1 and P0 is assigned to CPU2, the total time is 30ms.

   When all three programs are completed to one CPU, the total time is 35ms.

## Ex. 4 - Simple problem

For monochrome text screen, $25*80/1024=1.953125KB$, which costs $9.77.

For color bitmap, $1024*768*24/8/1024=2304KB$, which costs $11520.

For price today about or less than $10/GB, so that the price is much lower today.

## Ex. 5 - Command lines on a Unix system

```shell
useradd -m pastrol
ps -le
cat /proc/cpuinfo
cat /proc/meminfo
head -n 5 /dev/random | tee 1 > 2
cat 1 2 > 3
hexdump 3
grep -lR "ddekit_sem_down" /usr/src | grep "semaphore"
```
