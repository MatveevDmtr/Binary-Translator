#ifndef DSL_FOR_PARSER_H_INCLUDED
#define DSL_FOR_PARSER_H_INCLUDED

#define ACCUR 1

// pop rax
#define DO_POP_RAX                                                                \
{                                                                                 \
    cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);   \
    pop_node->bytecode |= rax;                                                    \
}

//pop rbx
#define DO_POP_RBX                                                                \
{                                                                                 \
    cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);   \
    pop_node->bytecode |= rbx;                                                    \
}

// add rax, rbx
#define DO_ADD_RAX_RBX                                                            \
{                                                                                 \
    cmd_t* op_node = NewNode(disasm, cmdlist, op, SIZE_ADD_REG_REG, ADD_REG_REG); \
    op_node->bytecode |= rbx << 19;                                               \
    op_node->bytecode |= rax << 16;                                               \
}

// sub rax, rbx
#define DO_SUB_RAX_RBX                                                            \
{                                                                                 \
    cmd_t* op_node = NewNode(disasm, cmdlist, op, SIZE_SUB_REG_REG, SUB_REG_REG); \
    op_node->bytecode |= rbx << 19;                                               \
    op_node->bytecode |= rax << 16;                                               \
}

//push rax
#define DO_PUSH_RAX                                                                \
{                                                                                  \
    cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);\
    push_node->bytecode |= rax;                                                    \
}


//pop rax
//cvtsi2sd xmm0, rax
#define DO_POP_XMM0                                                                                 \
{                                                                                                   \
    cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);                     \
    pop_node->bytecode |= rax;                                                                      \
    cmd_t* cvt_node = NewNode(disasm, cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM0_RAX, CVTSI2SD_XMM0_RAX); \
}

//pop rax
//cvtsi2sd xmm0, rax
#define DO_POP_XMM1                                                                                 \
{                                                                                                   \
    cmd_t* pop_node = NewNode(disasm, cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);                     \
    pop_node->bytecode |= rax;                                                                      \
    cmd_t* cvt_node = NewNode(disasm, cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM1_RAX, CVTSI2SD_XMM1_RAX); \
}

//mov rax, ACCUR
//cvtsi2sd xmm1, rax
#define DO_LOAD_1000_TO_XMM1                                                                        \
{                                                                                                   \
    cmd_t* mov_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM);             \
    mov_node->bytecode |= rax << 8;                                                                 \
    cmd_t* imm_node = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, ACCUR);                           \
    cmd_t* cvt_node = NewNode(disasm, cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM1_RAX, CVTSI2SD_XMM1_RAX); \
}

//div xmm0, xmm1
#define DO_DIV_XMM0_XMM1                                                                            \
{                                                                                                   \
    cmd_t* div_node = NewNode(disasm, cmdlist, CMD_DIVXMM, SIZE_DIVPD_XMM0_XMM1, DIVPD_XMM0_XMM1);  \
}

// mulpd xmm0, xmm1
#define DO_MUL_XMM0_XMM1                                                                            \
{                                                                                                   \
    cmd_t* mul_node = NewNode(disasm, cmdlist, CMD_MULXMM, SIZE_MULPD_XMM0_XMM1, MULPD_XMM0_XMM1);  \
}


//sqrtpd xmm0, xmm0
#define DO_SQRT_XMM0                                                                                \
{                                                                                                   \
    cmd_t* sqrt_node = NewNode(disasm, cmdlist, CMD_SQRT, SIZE_SQRTPD_XMM0_XMM0, SQRTPD_XMM0_XMM0); \
}


//cvtsd2si rax, xmm0
//push rax
#define DO_PUSH_XMM0                                                                                \
{                                                                                                   \
    cmd_t* cvt_node  = NewNode(disasm, cmdlist, CMD_CVT, SIZE_CVTSD2SI_RAX_XMM0, CVTSD2SI_RAX_XMM0);\
    cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);                 \
    push_node->bytecode |= rax;                                                                     \
}

// jmp addr
#define DO_COND_JUMP(name, mask)                                                                    \
{                                                                                                   \
    NewNode(disasm, cmdlist, name, SIZE_x86_COND_JMP, mask);                                        \
    NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, disasm->asm_code[disasm->ip++]);                   \
}

// pop rax
// pop rbx
// cmp rax, rbx 
#define DO_CMP                                                                                      \
{                                                                                                   \
    DO_POP_RAX;                                                                                     \
    DO_POP_RBX;                                                                                     \
    NewNode(disasm, cmdlist, CMD_CMP, SIZE_CMP_RAX_RBX, CMP_RAX_RBX);                               \
}

// jmp addr
#define DO_JUMP                                                                                     \
{                                                                                                   \
    NewNode(disasm, cmdlist, CMD_JMP, SIZE_x86_JMP, x86_JMP);                                       \
    NewNode(disasm, cmdlist, CMD_IMM, SIZE_ADDR, disasm->asm_code[disasm->ip++]);                   \
}

// mov rax, addr
// add rax, r14
// mov [rsi], rax
// mov rax, 8
// add rsi, rax
#define DO_CALL                                                                                                           \
{                                                                                                                         \
    cmd_t* rax_cur_adr = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM | (rax << 8));                   \
    cmd_t* adr_node    = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 0);                                                  \
    cmd_t* add_buf_adr = NewNode(disasm, cmdlist, CMD_ADD, SIZE_ADD_RAX_R14, ADD_RAX_R14);                                \
    cmd_t* mov_node    = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_RSI_OFFSET_RAX, MOV_RSI_OFFSET_RAX);                  \
    cmd_t* rax8_node   = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM | (rax << 8));                   \
    cmd_t* int_node    = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 8);                                                  \
    cmd_t* add_node    = NewNode(disasm, cmdlist, CMD_ADD, SIZE_ADD_REG_REG, (ADD_REG_REG | (rax << 19)) | (rsi << 16));  \
    DO_JUMP;                                                                                                              \
    adr_node->bytecode = cmdlist->tail->byteadr + cmdlist->tail->bytesize;                                                \
    log("RET ADR: %p\n");                                                                                                 \
}

// mov rax, 8
// sub rsi, rax
// mov rax, [rsi]
// push rax
#define DO_PUSH_RET_ADDR                                                                                                \
{                                                                                                                       \
    cmd_t* rax8_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM | (rax << 8));                   \
    cmd_t* int_node  = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 8);                                                  \
    cmd_t* sub_node  = NewNode(disasm, cmdlist, CMD_SUB, SIZE_SUB_REG_REG, (SUB_REG_REG | (rax << 19)) | (rsi << 16));  \
    cmd_t* mov_node  = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_RAX_RSI_OFFSET, MOV_RAX_RSI_OFFSET);                  \
    cmd_t* push_node = NewNode(disasm, cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG | rax);                               \
}

// mov r15, mem_addr
// mov rsi, mystack_addr
// mov r14, codebuf_addr
#define DO_MOV_MEM_ADDRS                                                                                        \
{                                                                                                               \
    cmd_t* mem_r15_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_R_REG_IMM, MOV_R_REG_IMM | r15 << 8);      \
    cmd_t* adr_mem_node = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 0);                                       \
                                                                                                                \
    cmd_t* mem_rsi_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM | rsi << 8);          \
    cmd_t* adr_stk_node = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 0);                                       \
                                                                                                                \
    cmd_t* mem_r14_node = NewNode(disasm, cmdlist, CMD_MOV, SIZE_MOV_R14_IMM, MOV_R14_IMM);                     \
    cmd_t* adr_buf_node = NewNode(disasm, cmdlist, CMD_IMM, SIZE_IMM, 0);                                       \
} // temporary nullptr, will be filled in executor

#endif //guard DSL_FOR_PARSER_H_INCLUDED