#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

#include <stdio.h>

typedef struct Disasm_struct
{
    int*        asm_code;
    size_t      ip;
    size_t      Size;
    const char* signature;
    char*       buf_code;
    size_t      cursor;
} disasm_t;

typedef struct Command
{
    const char*     name;
    size_t          bytesize;
    size_t          byteadr;
    char            bytecode[5];
    struct Command* next;
} cmd_t;

typedef struct CommandList
{
    cmd_t* head;
    cmd_t* tail;
    size_t size;
} cmdlist_t;


const size_t MAX_LEN_REG_NAME = 5;

cmdlist_t* CreateCmdList();

int CmdListDump(cmdlist_t* cmdlist);

int PutCmd(disasm_t* disasm, const char* cmd_name);

int HandleRegsDisasm(disasm_t* disasm, size_t reg_num);

int PutStackCmd(disasm_t* disasm);

int ParseByteCode(disasm_t* disasm, cmdlist_t* cmdlist);

int WriteUserCode(disasm_t* disasm, const char* filename);

int PutNumToCharBuf(disasm_t* disasm);

int checkSignDisasm(disasm_t* disasm, FILE* file_asm);

void BufCtor(disasm_t* disasm);

int getCodeForDisasm(disasm_t* disasm);

cmd_t* NewNode(cmdlist_t* cmdlist, const char* name, size_t bytesize, size_t byteadr, size_t bytecode);


#endif //DISASSEMBLER_H_INCLUDED
