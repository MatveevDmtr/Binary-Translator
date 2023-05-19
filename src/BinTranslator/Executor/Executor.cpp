#include <sys/mman.h>

#include "../Parser/parser.h"
#include "Executor.h"
#define PAGESIZE 4096


const size_t DEFAULT_BUFSIZE = 50;

int Execute(cmdlist_t* cmdlist)
{   
    codebuf_t codebuf = {};
    CodeBufCtor(&codebuf);
    
    FillCodeBuf(&codebuf, cmdlist);

    //char* buffer = codebuf.buf;

    CodeBufDump(&codebuf);

    RunBuffer(&codebuf);

    return 0;
}

int CodeBufCtor(codebuf_t* codebuf)
{
    codebuf->buf = (char*) aligned_alloc (PAGESIZE, DEFAULT_BUFSIZE * sizeof (char)); // alignment for mprotect
    Assert(codebuf->buf == nullptr);

    memset ((void*) codebuf->buf, 0x00, DEFAULT_BUFSIZE);      // Filling whole buffer with ret (0xC3)

    codebuf->cursor = 0;
    codebuf->capacity = DEFAULT_BUFSIZE;

    return 0;
}

int FillCodeBuf(codebuf_t* codebuf, cmdlist_t* cmdlist)
{
    //codebuf->buf[codebuf->cursor++] = bytecode;

    cmd_t* node = cmdlist->head;

    for(size_t i = 0; i < cmdlist->size; i++)
    {
        if (codebuf->cursor >= codebuf->capacity - node->bytesize)
        {
            int a = 0;
            //CodeBufResize();
        }

        strncpy(codebuf->buf + codebuf->cursor, node->bytecode, 5);

        codebuf->cursor += node->bytesize;

        log("cursor: %zd\n", codebuf->cursor);

        node = node->next;
    }

    return 0;
}

int RunBuffer(codebuf_t* codebuf)
{
    //log("PAGESIZE: %zd\n", PAGESIZE);
    mProtectChangeRights(codebuf, PROT_EXEC | PROT_READ | PROT_WRITE);

    void (*exec_buffer)() = (void (*)(void))(codebuf->buf);
    
    Assert(exec_buffer == nullptr);

    exec_buffer();

    mProtectChangeRights(codebuf, PROT_READ | PROT_WRITE);

    return 0;
}

int mProtectChangeRights(codebuf_t* codebuf, int new_rights)
{
    int mprotect_status = mprotect (codebuf->buf, codebuf->capacity + 1, new_rights);
    if (mprotect_status == -1)
    {
        print_log(FRAMED, "mProtectError");
        log("Can't change rights to memory at %p\n", codebuf->buf);
    }

    return mprotect_status;
}

int CodeBufDump(codebuf_t* codebuf)
{
    log("\n\n------------------------CodeBufDump------------------------\n\n");
    for(int i = 0; i < codebuf->cursor; i++)
    {
        log("%zx ", *(u_char*)(codebuf->buf + i));
        if (i % 16 == 15)   log("\n");
    }
    log("\n--------------------Finish CodeBufDump---------------------\n\n");

    return 0;
}