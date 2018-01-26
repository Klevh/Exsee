#ifndef EXSEE_MAIN_KLEVH_H
#define EXSEE_MAIN_KLEVH_H

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
 * @param name : name of the exception (used when instanciating using new_exsee)
 * @param what : text of the exception (of type char *)
 * @param size : size (in bytes) of the exception (if ommitted, will be equals to sizeof(exception_t)
 * @param constructor : function to be called when instanciating using new_exsee (of type void *(*)(void *,va_list))
 * @param destructor : function to be called when freed using delete_exsee (of type void *(*)(void *))
 */
#define NEW_EXCEPTION_EXSEE(name, args...) const exception_t name = {__COUNTER__, ##args}

/**
 * @brief Header part of NEW_EXCEPTION_EXSEE if not header only
 * @param name : type to be defined
 */
#define NEW_EXCEPTION_EXSEE_H(name) extern const exception_t name

/**
 * @brief Beginning of the TRY/CATCH/FINALLY blocks
 */
#define TRY if((++data_exsee.in_throw, !setjmp(data_exsee.env)))

/**
 * @brief Beginning of a catch block
 * @param type : exception type (either provided ones or created with NEW_EXCEPTION_EXSEE)
 * @param var : variable in which the exception should be saved
 */
#define CATCH(type, var) else if(data_exsee.exception && (type).id == ((exception_t*)data_exsee.exception)->id && ((var) = data_exsee.exception, 1))

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
	data_exsee.throwed = 1;			\
	data_exsee.exception = (e);			\
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

struct exsee_data{
    void * exception;
    int throwed;
    jmp_buf env;
    int signal;
    unsigned in_throw;
};

extern struct exsee_data data_exsee;

NEW_EXCEPTION_EXSEE_H(Exception);
NEW_EXCEPTION_EXSEE_H(AbortException);
NEW_EXCEPTION_EXSEE_H(FloatingPointException);
NEW_EXCEPTION_EXSEE_H(IllegalInstruction);
NEW_EXCEPTION_EXSEE_H(SignalInterupted);
NEW_EXCEPTION_EXSEE_H(SegmentationFault);
NEW_EXCEPTION_EXSEE_H(SignalTermination);
/* ------------------------------------------ */

/* ---------------- FUNCTIONS --------------- */
void on_signal_exsee(int signal);
void set_exsee(void);
/**
 * @brief instantiate an exception
 * @param type : type of the exception to be instantiated (either a provided one or a self-made made using NEW_EXCEPTION_EXSEE)
 * @param ... : parameters of the constructor, if any
 */
void * new_exsee(const exception_t type, ...);
/**
 * @brief free an instantiation of an exception
 * @param exception : instance to be freed
 */
void * delete_exsee(void * exception);
/* ------------------------------------------ */

/* ------------ MAIN REDIRECTION ------------ */
#ifndef NO_MAIN_EXSEE
#ifdef MAIN_ARGS_EXSEE
#undef MAIN_ARGS_EXSEE
#define MAIN_ARGS_EXSEE int argc, char ** argv
#define MAIN_CALL_EXSEE(argc, argv) main_exsee((argc),(argv))
#else
#define MAIN_ARGS_EXSEE
#define MAIN_CALL_EXSEE(argc,argv) main_exsee()
#endif

int main_exsee(MAIN_ARGS_EXSEE);
int main(MAIN_ARGS_EXSEE){
    set_exsee();
    return MAIN_CALL_EXSEE(argc, argv);
}
#define main main_exsee
#endif
/* ------------------------------------------ */
#endif
