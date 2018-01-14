#ifndef CEXCEPT_MAIN_KLEVH_H
#define CEXCEPT_MAIN_KLEVH_H

#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>

/*
 * Functions and macros that can be used by the user have doxygen comments,
 * others have not
 */

/* ----------------- MACROS ----------------- */
/**
 * @brief Create a new exception, all parameters are optionals, but if one is ommitted, all the following have to be ommitted too (to be used in header if header only, or in source file)
 * @param name : name of the exception (used when instanciating using new_cexcept)
 * @param what : text of the exception (of type char *)
 * @param size : size (in bytes) of the exception (if ommitted, will be equals to sizeof(exception_t)
 * @param constructor : function to be called when instanciating using new_cexcept (of type void *(*)(void *,va_list))
 * @param destructor : function to be called when freed using delete_cexcept (of type void *(*)(void *))
 */
#define NEW_EXCEPTION_CEXCEPT(name, ...) const exception_t name = {__COUNTER__, ##__VA_ARGS__}

/**
 * @brief Header part of NEW_EXCEPTION_CEXCEPT if not header only
 * @param name : type to be defined
 */
#define NEW_EXCEPTION_CEXCEPT_H(name) extern const exception_t name;

/**
 * @brief Beginning of the TRY/CATCH/FINALLY blocks
 */
#define TRY if((++data_cexcept.in_throw, !setjmp(data_cexcept.env)))

/**
 * @brief Beginning of a catch block
 * @param type : exception type (either provided ones or created with NEW_EXCEPTION_CEXCEPT)
 * @param var : variable in which the exception should be saved
 */
#define CATCH(type, var) else if(data_cexcept.exception && (type).id == ((exception_t*)data_cexcept.exception)->id && ((var) = data_cexcept.exception, 1))

/**
 * @brief Getter for the exception's message
 * @param e : instance of an exception
 * @return the message of the exception (of type char *)
 */
#define WHAT(e) (((exception_t*)(e))->what)

/**
 * @brief Throw an exception
 * @param e : exception to be thrown
 */
#define THROW(e) {					\
	data_cexcept.throwed = 1;			\
	data_cexcept.exception = (e);			\
	abort();					\
    }while(0)

/**
 * @brief Default catch block
 */
#define FINALLY else
/* ------------------------------------------ */

/* --------------- STRUCTURES --------------- */
typedef struct exception{
    unsigned id;
    char * what;
    size_t size;
    void * (*ctor)(void *, va_list);
    void * (*dtor)(void *);
}exception_t;

struct cexcept_data{
    void * exception;
    int throwed;
    jmp_buf env;
    int signal;
    unsigned in_throw;
};

extern struct cexcept_data data_cexcept;

NEW_EXCEPTION_CEXCEPT_H(Exception);
NEW_EXCEPTION_CEXCEPT_H(AbortException);
NEW_EXCEPTION_CEXCEPT_H(FloatingPointException);
NEW_EXCEPTION_CEXCEPT_H(IllegalInstruction);
NEW_EXCEPTION_CEXCEPT_H(SignalInterupted);
NEW_EXCEPTION_CEXCEPT_H(SegmentationFault);
NEW_EXCEPTION_CEXCEPT_H(SignalTermination);
/* ------------------------------------------ */

/* ---------------- FUNCTIONS --------------- */
void on_signal_cexcept(int signal);
void set_cexcept(void);
/**
 * @brief instantiate an exception
 * @param type : type of the exception to be instantiated (either a provided one or a self-made made using NEW_EXCEPTION_CEXCEPT)
 * @param ... : parameters of the constructor, if any
 */
void * new_cexcept(const exception_t type, ...);
/**
 * @brief free an instantiation of an exception
 * @param exception : instance to be freed
 */
void * delete_cexcept(void * exception);
/* ------------------------------------------ */

/* ------------ MAIN REDIRECTION ------------ */
#ifndef NO_MAIN_CEXCEPT
#ifdef MAIN_ARGS_CEXCEPT
#undef MAIN_ARGS_CEXCEPT
#define MAIN_ARGS_CEXCEPT int argc, char ** argv
#define MAIN_CALL_CEXCEPT(argc, argv) main_cexcept((argc),(argv))
#else
#define MAIN_ARGS_CEXCEPT
#define MAIN_CALL_CEXCEPT(argc,argv) main_cexcept()
#endif

int main_cexcept(MAIN_ARGS_CEXCEPT);
int main(MAIN_ARGS_CEXCEPT){
    set_cexcept();
    return MAIN_CALL_CEXCEPT(argc, argv);
}
#define main main_cexcept
#endif
/* ------------------------------------------ */
#endif
