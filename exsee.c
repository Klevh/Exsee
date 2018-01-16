#define NO_MAIN_EXSEE
#include "exsee.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

NEW_EXCEPTION_EXSEE(Exception, "Exception");
NEW_EXCEPTION_EXSEE(AbortException, "Abort Exception");
NEW_EXCEPTION_EXSEE(FloatingPointException, "Floating-point Exception");
NEW_EXCEPTION_EXSEE(IllegalInstruction, "Illegal Instruction");
NEW_EXCEPTION_EXSEE(SignalInterupted, "Signal Interupted");
NEW_EXCEPTION_EXSEE(SegmentationFault, "Segmentation Fault");
NEW_EXCEPTION_EXSEE(SignalTermination, "Signal Termination");

struct exsee_data data_exsee = {0};

void on_signal_exsee(int sig){
    if(data_exsee.throwed){
	data_exsee.signal = ((exception_t *)data_exsee.exception)->id;
	data_exsee.throwed = 0;
    }else{
	data_exsee.signal = sig;
	switch(sig){
	case SIGABRT:
	    data_exsee.exception = new_exsee(AbortException);
	    break;
	case SIGFPE:
	    data_exsee.exception = new_exsee(FloatingPointException);
	    break;
	case SIGILL:
	    data_exsee.exception = new_exsee(IllegalInstruction);
	    break;
	case SIGINT:
	    data_exsee.exception = new_exsee(SignalInterupted);
	    break;
	case SIGSEGV:
	    data_exsee.exception = new_exsee(SegmentationFault);
	    break;
	case SIGTERM:
	    data_exsee.exception = new_exsee(SignalTermination);
	    break;
	}
    }

    if(!data_exsee.in_throw){
	puts(WHAT(data_exsee.exception));
	delete_exsee(data_exsee.exception);
	signal(SIGABRT, NULL);
	abort();
    }
    data_exsee.throwed = 0;
    --data_exsee.in_throw;

    longjmp(data_exsee.env, data_exsee.signal);
}

void set_exsee(){
    signal(SIGABRT, &on_signal_exsee);
    signal(SIGFPE, &on_signal_exsee);
    signal(SIGILL, &on_signal_exsee);
    signal(SIGINT, &on_signal_exsee);
    signal(SIGSEGV, &on_signal_exsee);
    signal(SIGTERM, &on_signal_exsee);
}

void * new_exsee(const exception_t type, ...){
    void * p = malloc(type.size ? type.size : sizeof(exception_t));
    va_list vl;

    if(p){
	*(exception_t *)p = type;

	if(type.ctor){
	    va_start(vl, type);

	    p = type.ctor(p, vl);

	    va_end(vl);
	}
    }

    return p;
}

void * delete_exsee(void * self){
    const exception_t * ce = self;
    void * p;

    if(self && ce->dtor){
	p = ce->dtor(self);
    }

    free(self);

    return p;
}
