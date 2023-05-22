#include <sys/mman.h>

#include "../x86_commands/x86_commands.h"
#include "../Parser/parser.h"
#include "Executor.h"
#define PAGESIZE 4096


const size_t DEFAULT_BUFSIZE = 65536;
const size_t DEFAULT_MEMSIZE = 65536;
const size_t DEFAULT_STACK_CALLS_SIZE = 1024;

int ExecuteASM(cmdlist_t* cmdlist)
{   
    codebuf_t codebuf = {};
    CodeBufCtor(&codebuf);
    
    FillCodeBuf(&codebuf, cmdlist);

    char* buffer = codebuf.buf;

    CodeBufDump(&codebuf);

    RunBuffer(&codebuf);

    return 0;
}

int CodeBufCtor(codebuf_t* codebuf)
{
    codebuf->buf = (char*) aligned_alloc (PAGESIZE, DEFAULT_BUFSIZE * sizeof (char)); // alignment for mprotect
    Assert(codebuf->buf == nullptr);

    codebuf->memory = (char*) aligned_alloc (PAGESIZE, DEFAULT_MEMSIZE * sizeof (char));
    Assert(codebuf->memory == nullptr);

    codebuf->stkcalls = (char*) aligned_alloc (PAGESIZE, DEFAULT_STACK_CALLS_SIZE * sizeof (char));
    Assert(codebuf->stkcalls == nullptr);

    memset ((void*) codebuf->buf,      0x00, DEFAULT_BUFSIZE);               // Filling whole buffer with ret (0xC3)
    memset ((void*) codebuf->memory,   0x00, DEFAULT_MEMSIZE);               // Filling whole buffer with ret (0xC3)
    memset ((void*) codebuf->stkcalls, 0x00, DEFAULT_STACK_CALLS_SIZE);      // Filling whole buffer with ret (0xC3)

    codebuf->cursor        = 0;
    codebuf->capacity      = DEFAULT_BUFSIZE;
    codebuf->memsize       = DEFAULT_MEMSIZE;
    codebuf->stkcalls_size = DEFAULT_STACK_CALLS_SIZE;

    return 0;
}

int FillCodeBuf(codebuf_t* codebuf, cmdlist_t* cmdlist)
{
    FillAddrs_of_Memory_and_Funcs(cmdlist, codebuf);
    //codebuf->buf[codebuf->cursor++] = bytecode;

    cmd_t* node = cmdlist->head;

    for(size_t i = 0; i < cmdlist->size; i++)
    {
        if (codebuf->cursor >= codebuf->capacity - node->bytesize)
        {
            print_log(FRAMED, "StackOverFlow Error: the code size is too large");
        }

        *((uint64_t*)(codebuf->buf + codebuf->cursor)) = node->bytecode;


        codebuf->cursor += node->bytesize;

        log("cursor: %zd\n", codebuf->cursor);

        node = node->next;
    }

    //CmdListDump(cmdlist);

    return 0;
}

int FillAddrs_of_Memory_and_Funcs(cmdlist_t* cmdlist, codebuf_t* codebuf)
{
    cmd_t* addrmem_node = cmdlist->head->next;
    if (addrmem_node->name != CMD_IMM)          print_log(FRAMED, "BufferError: No Memory address found");
    addrmem_node->bytecode = (uint64_t)(codebuf->memory);
    log("\nchanged r15\n");

    cmd_t* addrstack_node = addrmem_node->next->next;
    if (addrstack_node->name != CMD_IMM)          print_log(FRAMED, "BufferError: No Stack address found");
    addrstack_node->bytecode = (uint64_t)(codebuf->stkcalls);
    log("changed rsi\n");

    cmd_t* addrbuf_node = addrstack_node->next->next;
    if (addrbuf_node->name != CMD_IMM)          print_log(FRAMED, "BufferError: No Buffer address found");
    addrbuf_node->bytecode = (uint64_t)(codebuf->buf);
    log("\nchanged r15\n");

    cmd_t* node = cmdlist->head;
    for(int i = 0; i < cmdlist->size; i++)
    {
        Assert(node == nullptr);

        if (node->name == CMD_CALL_IN || node->name == CMD_CALL_OUT)
        {
            log("Found call!\n");

            cmd_t* adr_node = node->next;
            
            if (node->name == CMD_CALL_IN)
            {
                adr_node->bytecode = (uint64_t)(ScanNumber) - (uint64_t)(codebuf->buf + adr_node->byteadr + adr_node->bytesize); // scanf
            }
            else if (node->name == CMD_CALL_OUT)
            {
                adr_node->bytecode = (uint64_t)(PrintNumber) - (uint64_t)(codebuf->buf + adr_node->byteadr + adr_node->bytesize); // printf
            }
        }                                                                                                                           

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
        log("%2zx ", *(u_char*)(codebuf->buf + i));
        if (i % 16 == 15)   log("\n");
    }
    log("\n--------------------Finish CodeBufDump---------------------\n\n");

    return 0;
}