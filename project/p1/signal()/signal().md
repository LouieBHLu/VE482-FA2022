#  signal()

## Description

- ````c
  #include <signal.h>
  ````

- ````c
  void (*signal(int sig, void (*func)(int)))(int)
  ````

## Args

- **sig** - signal code as an argument

|  Macro  |                            Signal                            |
| :-----: | :----------------------------------------------------------: |
| SIGABRT |            (Signal Abort) End program abnormally             |
| SIGFPE  |   (Signal Floating-Point Exception) Error in arithmetical    |
| SIGILL  |                 (Signal Illegal Instruction)                 |
| SIGINT  | (Signal Interrupt) Such as ctrl-C，usually generated by user |
| SIGSEGV | (Signal Segmentation Violation) Such as visiting non-existing object |
| SIGTERM |                      (Signal Terminate)                      |

- **func** - a function pointer; it can be customized, or predefined functions as follows:

|  Macro  |       Pointer        |
| :-----: | :------------------: |
| SIG_DFL | 默认的信号处理程序。 |
| SIG_IGN |      忽视信号。      |

## Return Value

- This function returns the value before the signal handler and **SIG_ERR** when an error occurs.
