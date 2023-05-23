#ifndef EXECUTOR_H_INCLUDED
#define EXECUTOR_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

#include "../Parser/parser.h"
#include "../../Common/logging/logging.hpp"

typedef struct codebuf
{
    char*  buf;
    size_t cursor;
    size_t capacity;
    char*  memory;
    size_t memsize;
    char*  stkcalls;
    size_t stkcalls_size;
} codebuf_t;

int ExecuteASM(cmdlist_t* cmdlist);

int FillCodeBuf(codebuf_t* codebuf, cmdlist_t* cmdlist);

int CodeBufCtor(codebuf_t* codebuf);

int CodeBufDump(codebuf_t* codebuf);

int RunBuffer(codebuf_t* codebuf);

int mProtectChangeRights(codebuf_t* codebuf, int new_rights);

int FillAddrs_of_Memory_and_Funcs(cmdlist_t* cmdlist, codebuf_t* codebuf);

int CodeBufDtor(codebuf_t* codebuf);

#endif //guard EXECUTOR_H_INCLUDED