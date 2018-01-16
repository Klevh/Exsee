# Exsee
A C version of TRY / CATCH / FINALLY blocks. Compatible with C89.
## Introduction
With this tool, you will be able to create self made exceptions and catch them to avoid your program to crash.  
Provided exceptions are :
* Exception : the generic exception
* AbortException : your program was about to crash abruptly
* FloatingPointException : a division by zero was done
* IllegalInstruction : you tried to modify the memory of the program itself while running (not the stack or the heap)
* SignalInterupted : the program was asked to stop (by an interruption character such as control-C)
* SegmentationFault : the good old segmentation fault was caught
* SignalTermination : the program was *asked* to stop
## How to catch an exception
Here is a commented example of how to catch an exception :
```c
#include "exsee.h"
#include <stdio.h>

int main(){
    void * a; // where you will store the exception
    int * b; // used as an example of segmentation fault exception

    TRY{
	      puts("ABORT TEST");
	      abort(); // here, we raise a AbortException
	      puts("failed"); // should not be printed
    }CATCH(AbortException, a){
	      puts(WHAT(a)); // WHAT returns the content of an exception
    }

    TRY{
      	puts("\nSEGFAULT TEST");
      	b = (int*)25;
        *b = 4; // here, we raise a SegmentationFault
        puts("failed"); // should not be printed
    }CATCH(SegmentationFault, a){
	      puts(WHAT(a)); // WHAT returns the content of an exception
    }

    return 0;
}
```
## How to create a self-made exception
Explaination coming soon ... For now, it is shown in the `tests` folder, in `main.c`.
