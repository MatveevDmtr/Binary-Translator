#include "../../Processor/cpu/cpu.h"
#include "parser.h"
#include "DSL_for_parser.h"
#include "../../Common/textbufs/textbufs.h"
#include "../x86_commands/x86_commands.h"

#undef DEF_CMD

#define DEF_CMD(name, num, arg, bytecode, bytesize, ...)  \
    case num:                                             \
        log("start translating cmd\n");                   \
        __VA_ARGS__                                       \
        disasm->buf_code[disasm->cursor++] = '\n';        \
    break;

//PutStackCmd(disasm);

//switch (arg)                                            \
        {                                                 \
            case 2:                                       \
                log("2 args in case\n");                  \
                                                          \
                break;                                    \
            case 1:                                       \
                WRITE_NUMBER;                             \
                break;                                    \
            default:                                      \
                log("No argument of cmd\n");              \
                __VA_ARGS__                               \
                log("finish VA_ARGS\n");                  \
        }                                                 \

#define WRITE_NUMBER                                      \
{                                                         \
                                                          \
        PutNumToCharBuf(disasm);                          \
}




const char* DISASM_SIGNATURE = "MDA";

const char* ASM_FILE_NAME_DISASM = "result/bytecodes/ASM.txt";

const int MAX_LEN_CMD = 6;

cmdlist_t* CreateCmdList()
{
    log("\n----------DISASM----------\n");

    disasm_t disasm = {NULL, 0, 0, DISASM_SIGNATURE, NULL, 0};

    log("disasm created\n");

    getCodeForDisasm(&disasm);

    log("Code was read\n");

    static cmdlist_t cmdlist = {.head = nullptr,
                                .tail = nullptr,
                                .size = 0};

    if (ParseByteCode(&disasm, &cmdlist))
    {
        print_log(FRAMED, "EXECUTION ERROR");
    }

    //WriteUserCode(&disasm, "result/Disassembled.txt");

    CmdListDump(&cmdlist);

    log("Finish disassembling\n");

    return &cmdlist;
}

cmd_t* NewNode(disasm_t* disasm, cmdlist_t* cmdlist, int name, size_t bytesize, uint64_t bytecode)
{
    cmd_t* cmd = (cmd_t*) calloc (1, sizeof(cmd_t));
    if  (cmd == nullptr)
    {
        print_log(FRAMED, "AllocationError: calloc in NewNode returned nullptr");
    }
    cmd->name     = name;
    cmd->bytesize = bytesize;

    cmd->bytecode = bytecode;

    if (cmdlist->tail != nullptr)   
    {
        cmdlist->tail->next = cmd;
        cmd->byteadr = cmdlist->tail->byteadr + cmdlist->tail->bytesize;
    }
    else
    {
        cmdlist->head = cmd;
        cmd->byteadr = 0;
    }
    cmdlist->tail = cmd;

    cmdlist->size++;

    cmd->ip = disasm->ip;

    return cmd;
}

int CmdListDump(cmdlist_t* cmdlist)
{
    Assert(cmdlist == nullptr);
    log("\n\n----------CmdList Dump----------\n");
    log("List size: %zd\n\n", cmdlist->size);

    cmd_t* node = cmdlist->head;

    for (size_t i = 0; i < cmdlist->size; i++)
    {
        log("~~~~~~~%.4s~~~~~~~\n", &(node->name));
        log("bytesize:    %zx\n", node->bytesize);
        log("bytecode:    %zx\n", node->bytecode);
        log("byteaddress: %zx\n", node->byteadr);
        log("ip:          %zx\n", node->ip);
        log("\n");
        node = node->next;
    }

    log("----------Finish CmdList Dump----------\n");

    return 0;
}

int PushHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    log("start PushHandler\n");

    size_t cmd_ip = disasm->ip++; //experiment

    log("code of push: %zd\n", disasm->asm_code[cmd_ip]);

    if (disasm->asm_code[cmd_ip] & ARG_RAM)
    {
        if (disasm->asm_code[cmd_ip] & ARG_IMMED)
        {
            cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_R15_OFFSET, PUSH_R15_OFFSET);
            cmd_t* imm_node  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, disasm->asm_code[disasm->ip++]);
        }
        
    }

    else if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);
        int reg_num = (disasm->asm_code)[disasm->ip++];
        push_node->bytecode |= reg_num;
    }

    else if (disasm->asm_code[cmd_ip] & ARG_IMMED)
    {
        cmd_t* mov_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM);
        mov_node->bytecode |= (rdi<< 8);
        cmd_t* imm_node  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, disasm->asm_code[disasm->ip++]);
        cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);
        push_node->bytecode |= rdi;
    }
    else
    {
        print_log(FRAMED, "Invalid Push argument\n");
    }
    log("finish PutStackArg\n");

    return 0;
}

int PopHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    log("start PopHandler\n");

    size_t cmd_ip = disasm->ip++; //experiment

    log("code of pop: %zd\n", disasm->asm_code[cmd_ip]);

    if (disasm->asm_code[cmd_ip] & ARG_RAM)
    {
        if (disasm->asm_code[cmd_ip] & ARG_IMMED)
        {
            cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_R15_OFFSET, POP_R15_OFFSET);
            cmd_t* imm_node  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, disasm->asm_code[disasm->ip++]);
        }
        
    }
    else if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);
        int reg_num = (disasm->asm_code)[disasm->ip++];
        pop_node->bytecode |= reg_num;
    }
    else
    {
        print_log(FRAMED, "Invalid Pop argument");
    }
    log("finish PutStackArg\n");

    return 0;
}

int ArythmeticHandler(disasm_t* disasm, cmdlist_t* cmdlist, uint64_t op)
{
    log("start ArythmeticHandler\n");

    disasm->ip++;

    switch (op)
    {
        case CMD_ADD:
            DO_POP_RBX;
            DO_POP_RAX;
            DO_ADD_RAX_RBX;
            DO_PUSH_RAX;
            break;

        case CMD_SUB:
            DO_POP_RBX;
            DO_POP_RAX;
            DO_SUB_RAX_RBX;
            DO_PUSH_RAX;
            break;

        case CMD_MUL:
            DO_POP_XMM0;
            DO_LOAD_1000_TO_XMM1;
            DO_DIV_XMM0_XMM1;
            DO_POP_XMM1;
            DO_MUL_XMM0_XMM1;
            DO_PUSH_XMM0;
            break;

        case CMD_DIV:
            DO_POP_XMM0;
            DO_POP_XMM1;
            DO_DIV_XMM0_XMM1;
            DO_LOAD_1000_TO_XMM1;
            DO_MUL_XMM0_XMM1;
            DO_PUSH_XMM0;
            break;

        case CMD_SQRT:
            DO_POP_XMM0;
            DO_SQRT_XMM0;
            DO_PUSH_XMM0;
            break;

        default:
            print_log(FRAMED, "Invalid Arythmetic operation");
            break;
    }

    return 0;
}

int CopyHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    disasm->ip++;

    DO_POP_RAX;
    DO_PUSH_RAX;
    DO_PUSH_RAX;

    return 0;
}

int JumpHandler(disasm_t* disasm, cmdlist_t* cmdlist, uint64_t op)
{
    size_t cmd_ip = disasm->ip++;
    
    switch (op)
    {
        case CMD_JMP:
            DO_JUMP;
            break;

        case CMD_JA:
            DO_CMP;
            DO_COND_JUMP(CMD_JA, x86_COND_JMP | (JG_MASK << 8));
            break;
        
        case CMD_JAE:
            DO_CMP;
            DO_COND_JUMP(CMD_JAE, x86_COND_JMP | (JGE_MASK << 8));
            break;

        case CMD_JB:
            DO_CMP;
            DO_COND_JUMP(CMD_JB, x86_COND_JMP | (JL_MASK << 8));
            break;

        case CMD_JBE:
            DO_CMP;
            DO_COND_JUMP(CMD_JBE, x86_COND_JMP | (JGE_MASK << 8));
            break;

        case CMD_JE:
            DO_CMP;
            DO_COND_JUMP(CMD_JE, x86_COND_JMP | (JE_MASK << 8));
            break;

        case CMD_JNE:
            DO_CMP;
            DO_COND_JUMP(CMD_JNE, x86_COND_JMP | (JNE_MASK << 8));
            break;

        case CMD_CALL:
            DO_CALL;
            break;
        
        default:
            print_log(FRAMED, "JumpError: invalid type of jump");
    }

    return 0;
}

cmd_t* SearchCmdByIp(cmdlist_t* cmdlist, size_t ip)
{
    cmd_t* node = cmdlist->head;
    for (size_t i = 0; i < cmdlist->size; i++)
    {
        if (node->ip >= ip)     return node;

        node = node->next;
    }

    print_log(FRAMED, "JumpError: jump to an invalid address");

    return nullptr;
}


int Skip()
{
    return 0;
}


int ParseByteCode(disasm_t* disasm, cmdlist_t* cmdlist)
{
    Assert(disasm  == NULL);
    Assert(cmdlist == nullptr);

    DO_MOV_MEM_ADDRS; // moving addresses of memory and StackCalls to r15 and rsi

    while (disasm->ip < disasm->Size)
    {
        log("disasm ip: %d\n", disasm->ip);
        log("cmd_code wo mask: %x\n", disasm->asm_code[disasm->ip]);

        switch(disasm->asm_code[disasm->ip] & CMD_MASK)
        {
            #include "../BinTransCodeGen/BinTransCodeGen.h"

            default:

                print_log(FRAMED, "COMMAND ERROR: Unexpected command");

                log("ERROR: Unexpected command, ip: %zd\n", disasm->ip);

                return UNDEFINED_CMD;
        }

        //CPUDump(cpu);
    }

    log("end of cycle\n");

    #undef DEF_CMD

    CmdListDump(cmdlist);

    FillJumpAddresses(cmdlist);

    return 0;
}

int RetHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    disasm->ip++;

    DO_PUSH_RET_ADDR;
    NewNode(disasm, cmdlist, CMD_RET, SIZE_x86_RET, x86_RET);

    return 0;
}

int CmdINHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    disasm->ip++;

    cmd_t* call_node = NewNode(disasm, cmdlist, CMD_CALL_IN, SIZE_x86_CALL, x86_CALL);         // call
    cmd_t* adr_call  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, 0);                     // adr 4 bytes
    cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG | rax);   // push rax
    
    //adr_call->bytecode = (uint64_t)(ScanNumber - adr_call->byteadr - adr_call->bytesize);

    return 0;
}

int CmdOUTHandler(disasm_t* disasm, cmdlist_t* cmdlist)
{
    disasm->ip++;

    cmd_t* pop_num   = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG | rdi);      // rdi = arg
    //cmd_t* arg_node  = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM | (rdi << 8));
    cmd_t* call_node = NewNode(disasm, cmdlist, CMD_CALL_OUT, SIZE_x86_CALL, x86_CALL);         // call
    cmd_t* adr_call  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, 0);                     // adr 4 bytes

    return 0;
}


int ScanNumber()
{
    int a = 0;
    scanf("%d", &a);
    
    return a;
}

int PrintNumber(long a)
{
    printf("Out Print is: %ld\n", a);
    return 0;
}

int FillJumpAddresses(cmdlist_t* cmdlist)
{
    log("\nStart Filling Jump Addresses\n");
    cmd_t* node = cmdlist->head;

    for(int i = 0; i < cmdlist->size; i++)
    {
        Assert(node == nullptr);

        if (node->name == CMD_JMP || node->name == CMD_JA || node->name == CMD_JAE || node->name == CMD_JB || node->name == CMD_JBE || 
            node->name == CMD_JE  || node->name == CMD_JNE)
        {
            log("Found jump!\n");
            cmd_t* adr_node = node->next;
            
            log("try to find ip %zd\n", adr_node->bytecode);

            log("Searched: %zd\n", (SearchCmdByIp(cmdlist, adr_node->bytecode))->byteadr);
            log("Addr of jump: %zd\n", node->byteadr);
            adr_node->bytecode = (SearchCmdByIp(cmdlist, adr_node->bytecode))->byteadr - adr_node->byteadr - adr_node->bytesize;        // jump adr|
        }                                                                                                                               // ________|
                                                                                                                                        // V destination
        node = node->next;
    }

    return 0;
}

int WriteUserCode(disasm_t* disasm, const char* filename)
{
    FILE* w_file = open_Wfile(filename);

    size_t num_written_sym = fwrite(disasm->buf_code,
                                    sizeof(char),
                                    disasm->cursor + 1,
                                    w_file);

    if (num_written_sym != disasm->cursor + 1)
    {
        print_log(FRAMED, "Error in writing to file\n");

        return -1;
    }

    log("File is written successfully\n");

    return 0;
}

int checkSignDisasm(disasm_t* disasm, FILE* file_asm)
{
    char asm_sign[MAX_LEN_SIGN] = {};

    int num_read_lines = fscanf(file_asm, "%3s", asm_sign);

    log("Lines read: %d\n", num_read_lines);

    log("sign read: %s, sign cpu: %s\n", asm_sign, disasm->signature);

    if (strcmp(asm_sign, disasm->signature))
    {
        print_log(FRAMED, "SIGNATURE ERROR");

        return -1;
    }

    return 0;
}

void BufCtor(disasm_t* disasm)
{
    disasm->asm_code = (int*)(calloc(disasm->Size, sizeof(int)));
    Assert(disasm->asm_code == nullptr);

    disasm->buf_code = (char*)(calloc(disasm->Size * MAX_LEN_CMD, sizeof(char)));
    Assert(disasm->buf_code == nullptr);
}

int getCodeForDisasm(disasm_t* disasm)
{
    FILE* file_asm = open_file_rmode(ASM_FILE_NAME_DISASM);

    Assert(file_asm == NULL);

    if (checkSignDisasm(disasm, file_asm))    return -1;

    size_t version = 0;

    fread(&version, sizeof(int), 1, file_asm);

    fread(&disasm->Size, sizeof(int), 1, file_asm);

    log("Size already read: %d\n", disasm->Size);

    BufCtor(disasm);

    fscanf(file_asm, "\n");

    fread(disasm->asm_code, sizeof(int), disasm->Size, file_asm);

    printf("\n");

    fclose(file_asm);

    return 0;
}
