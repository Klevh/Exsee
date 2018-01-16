#include "../exsee.h"
#include <stdio.h>

typedef struct myexcept{
    exception_t except;
    int i;
}myexcept_t;

void * ctor_me(void * e, va_list vl){
    ((myexcept_t *)e)->i = va_arg(vl, int);

    return e;
}

NEW_EXCEPTION_EXSEE(MyExcept,"Personnal exception", sizeof(myexcept_t),ctor_me);

int main(){
    void * a;
    int * b;

    TRY{
	puts("ABORT TEST");
	abort();
	puts("failed");
    }CATCH(AbortException, a){
	puts(WHAT(a));
    }

    TRY{
	puts("SEGFAULT");
	b = (int*)25;
	*b = 4;
	puts("failed");
    }CATCH(SegmentationFault, a){
	puts(WHAT(a));
    }

    TRY{
	puts("MyExcept");
	THROW(new_exsee(MyExcept, 1));
	puts("failed");
    }CATCH(MyExcept, a){
	puts(WHAT(a));
	delete_exsee(a);
    }

    TRY{
	puts("Finally");
	abort();
	puts("failed");
    }FINALLY{
	puts("succeeded");
    }

    THROW(new_exsee(MyExcept, 1));
    
    return 0;
}
