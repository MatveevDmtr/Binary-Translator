#ifndef DSL_FOR_PARSER_H_INCLUDED
#define DSL_FOR_PARSER_H_INCLUDED

#define ACCUR 1000

#define DO_POP_RAX                                                                \
{                                                                                 \
    cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);           \
    pop_node->bytecode |= rax;                                                    \
}
#define DO_POP_RBX                                                                \
{                                                                                 \
    cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);           \
    pop_node->bytecode |= rbx;                                                    \
}

#define DO_ADD_RAX_RBX                                                            \
{                                                                                 \
    cmd_t* op_node = NewNode(cmdlist, op, SIZE_ADD_REG_REG, ADD_REG_REG);         \
    op_node->bytecode |= rax << 19;                                               \
    op_node->bytecode |= rbx << 16;                                               \
}

#define DO_SUB_RAX_RBX                                                            \
{                                                                                 \
    cmd_t* op_node = NewNode(cmdlist, op, SIZE_SUB_REG_REG, SUB_REG_REG);         \
    op_node->bytecode |= rax << 19;                                               \
    op_node->bytecode |= rbx << 16;                                               \
}

#define DO_PUSH_RAX                                                                \
{                                                                                  \
    cmd_t* push_node = NewNode(cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);        \
    push_node->bytecode |= rax;                                                    \
}

#define DO_POP_XMM0                                                                                 \
{                                                                                                   \
    cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);                             \
    pop_node->bytecode |= rax;                                                                      \
    cmd_t* cvt_node = NewNode(cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM0_RAX, CVTSI2SD_XMM0_RAX);         \
}

#define DO_POP_XMM1                                                                                 \
{                                                                                                   \
    cmd_t* pop_node = NewNode(cmdlist, CMD_POP, SIZE_POP_REG, POP_REG);                             \
    pop_node->bytecode |= rax;                                                                      \
    cmd_t* cvt_node = NewNode(cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM1_RAX, CVTSI2SD_XMM1_RAX);         \
}

#define DO_LOAD_1000_TO_XMM1                                                                        \
{                                                                                                   \
    cmd_t* mov_node = NewNode(cmdlist, CMD_MOV, SIZE_MOV_REG_IMM, MOV_REG_IMM);                     \
    mov_node->bytecode |= rax << 8;                                                                 \
    cmd_t* imm_node = NewNode(cmdlist, CMD_IMM, SIZE_IMM, ACCUR);                                   \
    cmd_t* cvt_node = NewNode(cmdlist, CMD_CVT, SIZE_CVTSI2SD_XMM1_RAX, CVTSI2SD_XMM1_RAX);         \
}

#define DO_DIV_XMM0_XMM1                                                                            \
{                                                                                                   \
    cmd_t* div_node = NewNode(cmdlist, CMD_DIVXMM, SIZE_DIVPD_XMM0_XMM1, DIVPD_XMM0_XMM1);          \
}

#define DO_MUL_XMM0_XMM1                                                                            \
{                                                                                                   \
    cmd_t* mul_node = NewNode(cmdlist, CMD_MULXMM, SIZE_MULPD_XMM0_XMM1, MULPD_XMM0_XMM1);          \
}

#define DO_SQRT_XMM0                                                                                \
{                                                                                                   \
    cmd_t* sqrt_node = NewNode(cmdlist, CMD_SQRT, SIZE_SQRTPD_XMM0_XMM0, SQRTPD_XMM0_XMM0);         \
}

#define DO_PUSH_XMM0                                                                                \
{                                                                                                   \
    cmd_t* cvt_node  = NewNode(cmdlist, CMD_CVT, SIZE_CVTSD2SI_RAX_XMM0, CVTSD2SI_RAX_XMM0);        \
    cmd_t* push_node = NewNode(cmdlist, CMD_PUSH, SIZE_PUSH_REG, PUSH_REG);                         \
    push_node->bytecode |= rax;                                                                     \
}

#endif //guard DSL_FOR_PARSER_H_INCLUDED