#ifndef EXECUTOR_H_INCLUDED
#define EXECUTOR_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

#include "../../Common/logging/logging.hpp"

typedef struct codebuf
{
    char* buf;
    size_t cursor;
    size_t capacity;
} codebuf_t;

int Execute(cmdlist_t* cmdlist);

int FillCodeBuf(codebuf_t* codebuf, cmdlist_t* cmdlist);

int CodeBufCtor(codebuf_t* codebuf);

int CodeBufDump(codebuf_t* codebuf);

int RunBuffer(codebuf_t* codebuf);

int mProtectChangeRights(codebuf_t* codebuf, int new_rights);

#endif //guard EXECUTOR_H_INCLUDED