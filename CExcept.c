#define NO_MAIN_CEXCEPT
#include "CExcept.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

NEW_EXCEPTION_CEXCEPT(Exception, "Exception");
NEW_EXCEPTION_CEXCEPT(AbortException, "Abort Exception");
NEW_EXCEPTION_CEXCEPT(FloatingPointException, "Floating-point Exception");
NEW_EXCEPTION_CEXCEPT(IllegalInstruction, "Illegal Instruction");
NEW_EXCEPTION_CEXCEPT(SignalInterupted, "Signal Interupted");
NEW_EXCEPTION_CEXCEPT(SegmentationFault, "Segmentation Fault");
NEW_EXCEPTION_CEXCEPT(SignalTermination, "Signal Termination");

struct cexcept_data data_cexcept = {0};

void on_signal_cexcept(int sig){
    if(data_cexcept.throwed){
	data_cexcept.signal = ((exception_t *)data_cexcept.exception)->id;
	data_cexcept.throwed = 0;
    }else{
	data_cexcept.signal = sig;
	switch(sig){
	case SIGABRT:
	    data_cexcept.exception = new_cexcept(AbortException);
	    break;
	case SIGFPE:
	    data_cexcept.exception = new_cexcept(FloatingPointException);
	    break;
	case SIGILL:
	    data_cexcept.exception = new_cexcept(IllegalInstruction);
	    break;
	case SIGINT:
	    data_cexcept.exception = new_cexcept(SignalInterupted);
	    break;
	case SIGSEGV:
	    data_cexcept.exception = new_cexcept(SegmentationFault);
	    break;
	case SIGTERM:
	    data_cexcept.exception = new_cexcept(SignalTermination);
	    break;
	}
    }

    if(!data_cexcept.in_throw){
	puts(WHAT(data_cexcept.exception));
	delete_cexcept(data_cexcept.exception);
	signal(SIGABRT, NULL);
	abort();
    }
    data_cexcept.throwed = 0;
    --data_cexcept.in_throw;

    longjmp(data_cexcept.env, data_cexcept.signal);
}

void set_cexcept(){
    signal(SIGABRT, &on_signal_cexcept);
    signal(SIGFPE, &on_signal_cexcept);
    signal(SIGILL, &on_signal_cexcept);
    signal(SIGINT, &on_signal_cexcept);
    signal(SIGSEGV, &on_signal_cexcept);
    signal(SIGTERM, &on_signal_cexcept);
}

void * new_cexcept(const exception_t type, ...){
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

void * delete_cexcept(void * self){
    const exception_t * ce = self;
    void * p;

    if(self && ce->dtor){
	p = ce->dtor(self);
    }

    free(self);

    return p;
}
