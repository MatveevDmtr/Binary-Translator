#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "Cpu_Config.h"
#include "../logging/logging.hpp"
#include "../textbufs/textbufs.h"

#define RUN_MODE

#define DEF_CMD(name, num, arg, ...) \
    case num:                        \
    __VA_ARGS__                      \
    break;

#define RETURN_PTR_ARG(arg)          \
{                                    \
    cpu->ip--;                       \
                                     \
    return arg;                      \
}

#define allocate_array(type, num_elems)                             \
({                                                                  \
    type* location__ = (type*) calloc (num_elems, sizeof (type));   \
    log ("Type_calloc: %s\n", #type);                               \
    log ("Sizeof type: %d\n", sizeof (type*));                      \
    Assert(location__ == NULL);                                     \
    log ("Calloc returned: %p\n\n", location__);                    \
    location__;                                                     \
})

#define FREE(ptr)                                                   \
{                                                                   \
    Assert(ptr == NULL);                                            \
                                                                    \
    free(ptr);                                                      \
}

const int CMD_MASK = 31; /*11111*/

const size_t MAX_LEN_SIGN = 20;

const int ACCURACY = 1;

enum CPU_ERRORS
{
    WRONG_SIGNATURE   = 1,
    UNDEFINED_CMD     = 2,
    ZERO_DIVISION     = 3,
    INVALID_REG_INDEX = 4,
    INVALID_MEM_INDEX = 5
};

enum PrintRAMFromats
{
    BIN_FORMAT = 0,
    SYM_FORMAT = 1,
    INT_FORMAT = 2
};


#endif //guard
