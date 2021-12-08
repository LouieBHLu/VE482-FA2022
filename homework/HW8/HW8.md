## Ex.1 - Input/Output

1. **Is it possible to scan a document and send it through a wireless network at full speed? Explain why, why not.**

   No. In reality, scanning the document involves a lot of I/O bound work, so that the process of the wireless network cannot send out the data at full speed.

2. **How can an OS facilitate the installation of a new device without any need to recompile the OS or even to reboot it?**

   Use loadable driver, so that the new device can be loaded dynamically, and the system don't need to be recompiled.

3. **In the slides four I/O software layers are listed; in which layers are the following operations done:**

   a) Device Independent OS Software

   b) Device Drivers

   c) Interrupt Handlers

   d) User Level I/O Software

4. **A DMA controller has four channels. The controller is able to request a 32bit word every 100 nsec. A respond takes equally long. How fast should the bus be in order to avoid being a bottleneck?**

   $2\times10^7$ byte/s

5. **Briefly explain what a thin client is.**

   A thin client is a computer that runs from resources stored on a central server instead of a localized hard drive. Thin clients work by connecting remotely to a server-based computing environment where most applications, sensitive data, and memory, are stored.

6. **Describe how a hard disk is working and give a few details regarding its geometry.**

   The hard drive contains a **spinning platter** with a thin magnetic coating. A "head" moves over the platter, writing 0's and 1's as tiny areas of magnetic North or South on the platter. To read the data back, the head goes to the same spot, notices the North and South spots flying by, and so deduces the stored 0's and 1's.

   There are paths on the surface of the head of the disk in a shape of cylinder.

7. **What are differences between RAID 0, 1, 5, 6, and 10?** [^1]

   **RAID 0**: In a RAID 0 system, data are split up into blocks that get written across all the drives in the array. By using multiple disks (at least 2) at the same time, this offers fast read and write speeds. All storage capacity can be fully used with no overhead.

   **RAID 1**: a setup of at least two drives that contain the exact same data. If a drive fails, the others will still work. It is recommended for those who need high reliability.

   **RAID 5**: requires the use of at least 3 drives, striping the data across multiple drives like RAID 0, but also has a “parity” distributed across the drives. In the event of a single drive failure, data is pieced together using the parity information stored on the other drives. There is zero downtime. Read speed is very fast but write speed is somewhat slower due to the parity that has to be calculated.

   **RAID 6**: similar with RAID 5, but the parity data are written to two drives. That means it requires at least 4 drives and can withstand 2 drives dying simultaneously. Read speed is as fast as RAID 5, but write speed is slower than RAID 5 due to the additional parity data that have to be calculated.

   **RAID 10**: consists of a minimum for four drives and combine the advantages of RAID 0 and RAID 1 in one single system. It provides security by mirroring all data on secondary drives while using striping across each set of drives to speed up data transfers. This means that RAID 10 can provide the speed of RAID 0 with the redundancy of RAID 1.

## Ex.2 - Multiprocessors

1. **Suppose that the TSL instruction was not available for synchronizing a multiprocessor. Instead, another instruction, SWP was provided that atomically swapped the contents of a register with a word in memory. How could that be used to provide multiprocessor synchronization?**

   first load 1 into the register, then swap register with memory, so that the memory has value 1, indicating the data is locked. That can be utilized to provide multiprocessor synchronization.

2. **Briefly describe virtualization and the main challenges that need to be overcome.** [^2]

   In computing, virtualization or virtualisation is the act of creating a virtual version of something, including virtual computer hardware platforms, storage devices, and computer network resources. 

   The major challenges needed to be overcome: Resource distribution, VM Sprawl, Backward compatibility, Performance monitoring, Backup, Security.

## Ex.3 - File Systems

1. **Assuming the current working directory is /usr/share/doc/mutt/examples what is the absolute path of ../../../doc/../../lib/mutt?**

   `/usr/lib/mutt`

2. **A Unix system has 1KB blocks and 4B disk addresses. What is the maximum file size if i-nodes contain 13 direct entries, and one single, double, and triple indirect entry each?**

   $13+256+256^2+256^3=16843021KB$

3. **The time required to read a certain number of bytes can be approximated by summing the seek, rotation delay, and transfer times. Given a disk with mean seek time of 8 msec, a rotation rate of 15,000 rpm and 262,144 bytes per track what are the data rates for block sizes of (i) 1 KB, (ii) 2 KB, and (iii) 4 KB?**

   We can get that 15000 rpm -> 2.00ms.

   (i) $8+2+(1\times1024/2262144)*1000=12.453$

   (ii) $8+2+(2\times1024/2262144)*1000=12.905$

   (iii) $8+2+(4\times1024/2262144)*1000=13.811$

## Ex.4 - Security

1. **If using only lowercase letters how many possible 8 letter words can be chosen? Explain why a password should always contain lower and uppercase letters, numbers and symbols.**

   $26*25...*19=62990928000$ possible words, which is not a large number for computer. A password containing both lower and uppercase letters would be harder to break.

2. **Explain why memory dynamically allocated using `malloc` is sometimes random and some other times set to 0.**

   For security reasons. Return a 0 can protect the data location from leak.

3. **A computer system is made of several components each featuring a different level of security. How secure is this system?**

   If it's referred to a system that is adequate to protect itself from subversion and has robust mechanisms to separate information domains, that is, trustworthy. [^3]

4. **Can security be achieved? Explain your answer.**

   Not really. Every system has certain flaws that can be targeted at and reserving security costs a lot of resources.

## Ex.5 - Research

A Linux firewall is a solution or service that regulates, protects, and blocks network traffic as it passes to and from a Linux-based environment. Most Linux distributions, including Debian, Ubuntu, CentOS, etc., ship with pre-built firewall services of their own (much like Microsoft Windows has Windows Defender firewall turned on by default).

`iptables` is a user-space utility program that allows a system administrator to configure the IP packet filter rules of the Linux kernel firewall, implemented as different Netfilter modules. The filters are organized in different tables, which contain chains of rules for how to treat network traffic packets. Different kernel modules and programs are currently used for different protocols; iptables applies to IPv4, ip6tables to IPv6, arptables to ARP, and ebtables to Ethernet frames. [^4]

`ebtables` is an application program used to set up and maintain the tables of rules (inside the Linux kernel) that inspect Ethernet frames. It is analogous to the `iptables`application, but less complicated, due to the fact that the Ethernet protocol is much simpler than the IP protocol. [^5]

`arptables` is used to set up, maintain, and inspect the tables of ARP packet filter rules in the Linux kernel. Several different tables may be defined. Each table contains a number of built-in chains and may also contain user-defined chains. Each chain is a list of rules which can match a set of packets. Each rule specifies what to do with a packet that matches. This is called a 'target', which may be a jump to a user-defined chain in the same table. [^6]

## Ex.6 - Linux

A kernel panic is a safety measure taken by an operating system's kernel upon detecting an internal fatal error in which either it is unable to safely recover or continuing to run the system would have a higher risk of major data loss.

Refer to [^7], we find that a single command can trigger kernel panic.	

```shell
echo c > /proc/sysrq-trigger
```

a kernel panic will be triggered.

<img src="C:\Users\Lenovo\Desktop\kernel panic.png" alt="kernel panic" style="zoom:50%;" />

## Ex7 Survey

Done.

![482 eva](C:\Users\Lenovo\Desktop\482 eva.png)

## Reference

[^1]: [RAID Level Comparison: RAID 0, RAID 1, RAID 5, RAID 6 and RAID 10 (dataplugs.com)](https://www.dataplugs.com/en/raid-level-comparison-raid-0-raid-1-raid-5-raid-6-raid-10/)

[^2]: [What are the common challenges of virtualization? (techadvisory.org)](https://www.techadvisory.org/2019/05/what-are-the-common-challenges-of-virtualization/)
[^3]: [Multilevel security - Wikipedia](https://en.wikipedia.org/wiki/Multilevel_security)
[^4]: [iptables - Wikipedia](https://en.wikipedia.org/wiki/Iptables)
[^5]: [ebtables(8) - Linux man page (die.net)](https://linux.die.net/man/8/ebtables)
[^6]: [arptables(8) - Linux man page (die.net)](https://linux.die.net/man/8/arptables)
[^7]: [How to cause kernel panic with a single command? - Unix & Linux Stack Exchange](https://unix.stackexchange.com/questions/66197/how-to-cause-kernel-panic-with-a-single-command)

