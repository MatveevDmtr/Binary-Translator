#include "../../Processor/cpu/cpu.h"
#include "parser.h"
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

    CmdListDump(&cmdlist);

    //WriteUserCode(&disasm, "result/Disassembled.txt");

    log("Finish disassembling\n");

    return &cmdlist;
}

// void Push_Handler(Byte_Code_Node* byte_node, x86_Nodes_List* x86_list, int* code_size)
// {
//     if (byte_node->mem_arg.has_arg)
//     {
//         SET_COMAND(PUSH_R15_OFFSET);
//         SET_IMM_ARG_MEM();                      // ????? ?????? ?????????? ???????? ?????? ?? + code size
//     }

//     else
//     {
//         SET_COMAND(PUSH_REG, | (byte_node->reg_arg.arg));
//     }

//     SET_OLD_IP();
//     NEXT_NODE();
// }


cmd_t* NewNode(cmdlist_t* cmdlist, int name, size_t bytesize, size_t byteadr, uint64_t bytecode)
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
        log("bytesize: %zx\n", node->bytesize);
        log("bytecode: %zx\n", node->bytecode);
        log("byteaddress: %zx\n", node->byteadr);
        log("\n");
        node = node->next;
    }

    log("----------Finish CmdList Dump----------\n");

    return 0;
}

int PutCmd(disasm_t* disasm, const char* cmd_name)
{
    strcpy(disasm->buf_code + disasm->cursor, cmd_name);

    log("finish strcpy of %s\n", cmd_name);

    disasm->cursor += strlen(cmd_name);

    disasm->buf_code[disasm->cursor++] = ' ';

    disasm->ip++; //experiment

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
            cmd_t* push_node = NewNode(cmdlist, CMD_PUSH, SIZE_PUSH_R15_OFFSET, 0, PUSH_R15_OFFSET);
            cmd_t* imm_node  = NewNode(cmdlist, CMD_IMM, SIZE_IMM, 0, disasm->asm_code[disasm->ip++]);
        }
        
    }

    else if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        cmd_t* push_node = NewNode(cmdlist, CMD_PUSH, SIZE_PUSH_REG, 0, PUSH_REG);
        int reg_num = (disasm->asm_code)[disasm->ip++];
        push_node->bytecode |= reg_num;
    }

    else if (disasm->asm_code[cmd_ip] & ARG_IMMED)
    {
        cmd_t* mov_node = NewNode(cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, 0, MOV_REG_IMM);
        mov_node->bytecode |= r14 << 8;
        cmd_t* imm_node  = NewNode(cmdlist, CMD_IMM, SIZE_IMM, 0, disasm->asm_code[disasm->ip++]);
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
    log("start PushHandler\n");

    size_t cmd_ip = disasm->ip++; //experiment

    log("code of push: %zd\n", disasm->asm_code[cmd_ip]);

    if (disasm->asm_code[cmd_ip] & ARG_RAM)
    {
        if (disasm->asm_code[cmd_ip] & ARG_IMMED)
        {
            cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_R15_OFFSET, 0, POP_R15_OFFSET);
            cmd_t* imm_node  = NewNode(cmdlist, CMD_IMM, SIZE_IMM, 0, disasm->asm_code[disasm->ip++]);
        }
        
    }
    else if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_REG, 0, POP_REG);
        int reg_num = (disasm->asm_code)[disasm->ip++];
        pop_node->bytecode |= reg_num;
    }
    else
    {
        print_log(FRAMED, "Invalid Pop argument\n");
    }
    log("finish PutStackArg\n");

    return 0;
}

int HandleRegsDisasm(disasm_t* disasm, size_t reg_num, cmd_t* node)
{
    char reg_name[MAX_LEN_REG_NAME] = {'r', 'z', 'x'};

    reg_name[1] = (char)(reg_num + (int)'a');

    log("reg_name: %s\n\n", reg_name);

    strcpy(disasm->buf_code + disasm->cursor, reg_name);

    disasm->cursor += strlen(reg_name);

    return 0;
}

int PutNumToCharBuf(disasm_t* disasm)
{
    char line[5] = {};
    snprintf(line, 5, "%d", (disasm->asm_code)[disasm->ip]);
    strncpy(disasm->buf_code + disasm->cursor, line, 5);

    disasm->cursor += strlen(line);

    disasm->ip++;

    return 0;
}

int Skip()
{
    return 0;
}

int PutStackCmd(disasm_t* disasm, cmd_t* node)
{
    log("start PutStackArg\n");

    size_t cmd_ip = disasm->ip - 1; //experiment

    log("code of push: %d\n", disasm->asm_code[cmd_ip]);

    if (disasm->asm_code[cmd_ip] & ARG_RAM)   disasm->buf_code[disasm->cursor++] = '[';

    if (disasm->asm_code[cmd_ip] & ARG_REG)
    {
        HandleRegsDisasm(disasm, (disasm->asm_code)[disasm->ip++], node);
    }

    if (disasm->asm_code[cmd_ip] & ARG_IMMED)
    {
        WRITE_NUMBER;
    }

    if (disasm->asm_code[cmd_ip] & ARG_RAM)   disasm->buf_code[disasm->cursor++] = ']';

    log("finish PutStackArg\n");

    return 0;
}


int ParseByteCode(disasm_t* disasm, cmdlist_t* cmdlist)
{
    Assert(disasm == NULL);

    log("ip: %d, Size: %d\n", disasm->ip, disasm->Size);

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
