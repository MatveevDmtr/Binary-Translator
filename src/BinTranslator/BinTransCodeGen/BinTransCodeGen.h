#ifndef COMMANDS_FOR_CPU_H_INCLUDED
#define COMMANDS_FOR_CPU_H_INCLUDED

#define DO_JUMP        {cpu->ip = cpu->code[(cpu->ip)+1] - 2;}

#define DO_POP         ({(double)StackPop(stk) / ACCURACY;})

#define DO_PUSH(arg)   StackPush(stk, (int)((arg) * ACCURACY));

#define REMEMBER_CALL  {StackPush(cpu->StkCalls, cpu->ip);};

#define RETURN_TO_CALL {cpu->ip = StackPop(cpu->StkCalls) + 1;};

#define SKIP_ARG       {(cpu->ip)++;};

#define COND_JUMP(condition)                                 \
{                                                            \
    if (condition)                                           \
    {                                                        \
        DO_JUMP;                                             \
    }                                                        \
    else                                                     \
    {                                                        \
        SKIP_ARG;                                            \
    }                                                        \
}

// for assembler

#define WRITE_CMD_NUM   {(asm_code->Ptr)[(asm_code->Ip)++] = cmd_code;};

#define WRITE_ARG_JUMP  {FuncJump(cursor, asm_code);};

#define WRITE_STACK_ARG {PutArg(cmd_code, cursor, asm_code);};

#define WRITE_JUMP                                                           \
{                                                                            \
    WRITE_CMD_NUM;                                                           \
    WRITE_ARG_JUMP;                                                          \
}

DEF_CMD(HLT, 0, 0, x86_RET, SIZE_x86_RET, 
NewNode(cmdlist, CMD_RET, SIZE_x86_RET, 0, x86_RET);
disasm->ip++;
)

DEF_CMD(PUSH, 1, 2, 0, SIZE_PUSH_REG, 0,
PushHandler(disasm, cmdlist);
)

DEF_CMD(POP, 2, 2, 0, 0,
PopHandler(disasm, cmdlist);
)

DEF_CMD(ADD , 3, 0, 0, 0,
Skip();//ArythmeticHandler();
)

DEF_CMD(SUB , 4, 0, 0, 0,
Skip();//ArythmeticHandler();
)

DEF_CMD(MUL , 5, 0, 0, 0,
Skip();
)

DEF_CMD(DIV , 6, 0, 0, 0,
Skip();
)

DEF_CMD(IN  , 7, 0, 0, 0,
Skip();
)

DEF_CMD(OUT , 8, 0, 0, 0,
Skip();
)

DEF_CMD(JUMP, 9, 1, 0, 0,
Skip();
)

DEF_CMD(JB, 10, 1, 0, 0,
Skip();
)

DEF_CMD(JBE, 11, 1, 0, 0,
Skip();
)

DEF_CMD(JA, 12, 1, 0, 0,
Skip();
)

DEF_CMD(JAE, 13, 1, 0, 0,
Skip();
)

DEF_CMD(JE, 14, 1, 0, 0,
Skip();
)

DEF_CMD(JNE, 15, 1, 0, 0,
Skip();
)
//directive
// DEF_CMD(NAME, 16, 0, 0, 0,
// Skip();
// )
// //directive
// DEF_CMD(LBL , 17, 0, 0, 0,
// Skip();
// )

DEF_CMD(DRAW, 18, 0, 0, 0,
Skip();
)

DEF_CMD(CALL, 19, 1, 0, 0,
Skip();
)

DEF_CMD(RET, 20, 0, x86_RET, SIZE_x86_RET, 
NewNode(cmdlist, CMD_RET, SIZE_x86_RET, 0, x86_RET);
disasm->ip++;
)

DEF_CMD(COPY, 21, 0, 0, 0,
Skip();
)

DEF_CMD(SQRT, 22, 0, 0, 0,
Skip();
)

DEF_CMD(CLEAR, 23, 0, 0, 0,
Skip();
)

#endif // COMMANDS_FOR_CPU_H_INCLUDED
