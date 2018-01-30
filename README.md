# Exsee
A C version of TRY / CATCH / FINALLY blocks. Compatible with GNU C89.
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
To explain how to create a self-made exception, we will create one containing an `int` attribute.  
First, we need to create our structure that will _inerit_ from `exception_t` like below :
```c
typedef struct myexcept{
    exception_t except;
    int i;
}myexcept_t;
```
With this class, we will now _instanciate_ the `except` attribute of our class (we do not really instanciate, we rather define how each instance will be instanciated). For that, we will do like below :
```c
// In the .c file
NEW_EXCEPTION_EXSEE(MyExcept, // Name of the exception (name to use with new_exsee, delete_exsee and catch)
                    "Personnal exception", // WHAT message of the exception
		    sizeof(myexcept_t), // memory size of the exception (can be set to 0 if it is the same size as exception_t)
		    ctor_me, // constructor of type void*(*)(void*,va_list) (can be NULL)
		    NULL); // destructor of type void*(*)(void) (can be NULL)

// In the .h file
NEW_EXCEPTION_EXSEE_H(MyExcept);
```
If we want to instanciate and delete an instance of our newly made exception (well, there is a missing function but it is described later), we can do like that :
```c
void * except = new_exsee(MyExcept, 1); // can be of type myexcept_t *
delete_exsee(except);
```
The last step (that will appeare before in your code) is the implementation of a constructor. It is really simple and can be done just like that :
```c
void * ctor_me(void * e, va_list vl){
    ((myexcept_t *)e)->i = va_arg(vl, int);

    return e;
}
```
