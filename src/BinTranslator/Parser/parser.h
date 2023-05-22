#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>

//#include "../Executor/Executor.h"

const size_t MAX_BYTECODE_LEN = 15;

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
    int             name;
    size_t          bytesize;
    size_t          byteadr;
    uint64_t        bytecode;
    size_t          ip; 
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

int Skip();

int CmdListDump(cmdlist_t* cmdlist);

int PutCmd(disasm_t* disasm, const char* cmd_name);

int HandleRegsDisasm(disasm_t* disasm, size_t reg_num, cmd_t* node);

int PutStackCmd(disasm_t* disasm, cmd_t* node);

int ParseByteCode(disasm_t* disasm, cmdlist_t* cmdlist);

int WriteUserCode(disasm_t* disasm, const char* filename);

int PutNumToCharBuf(disasm_t* disasm);

int checkSignDisasm(disasm_t* disasm, FILE* file_asm);

void BufCtor(disasm_t* disasm);

int getCodeForDisasm(disasm_t* disasm);

cmd_t* NewNode(disasm_t* disasm, cmdlist_t* cmdlist, int name, size_t bytesize, uint64_t bytecode);

int PushHandler(disasm_t* disasm, cmdlist_t* cmdlist);

int PopHandler(disasm_t* disasm, cmdlist_t* cmdlist);

int ArythmeticHandler(disasm_t* disasm, cmdlist_t* cmdlist, uint64_t op);

int CopyHandler(disasm_t* disasm, cmdlist_t* cmdlist);

int RetHandler(disasm_t* disasm, cmdlist_t* cmdlist);

cmd_t* SearchCmdByIp(cmdlist_t* cmdlist, size_t ip);

int FillJumpAddresses(cmdlist_t* cmdlist);

int ScanNumber();

int PrintNumber(int a);

int CmdOUTHandler(disasm_t* disasm, cmdlist_t* cmdlist);

int CmdINHandler(disasm_t* disasm, cmdlist_t* cmdlist);


#endif //DISASSEMBLER_H_INCLUDED
