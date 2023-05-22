#pragma GCC diagnostic ignored "-Wmultichar"

enum Registers
{
    rax,
    rcx,
    rdx,
    rbx,
    rbp,
    rsp,
    rsi,
    rdi,
};

enum x86_Commands : u_int64_t
{
    // mov r_x, imm
    MOV_REG_IMM = 0xb848,                // "|" with shifted by 8 reg mask

    MOV_REG_REG = 0xc08948,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    // mov r_x, [r15 + offset]
    MOV_REG_R15_OFFSET = 0x878b49,      // "|" with shifted left by 19 reg
                                        //followed by int offset

    // mov [r15 + offset], r_x
    MOV_R15_OFFSET_REG = 0x878949,      // the same

    PUSH_REG = 0x50,                     // "|" with reg mask

    // push [r15 + offset]
    PUSH_R15_OFFSET = 0xb7ff41,          // followed by int offset

    POP_REG = 0x58,                     // "|" with reg mask

    // pop [r15 + offset]
    POP_R15_OFFSET = 0x878f41,          // followed by int offset

    ADD_REG_REG = 0xc00148,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    SUB_REG_REG = 0xc02948,             // first reg - "|" with shifted by 19 mask
                                        // second reg - "|" with shifted by 16 mask

    IMUL_REG_REG = 0xc0af0f48,          // first reg - "|" with shifted by 27 mask
                                        // second reg - "|" with shifted by 24 mask

    CQO = 0x9948,                       // signed rax -> (rdx, rax)

    IDIV_REG = 0xf8f748,                // "|" with reg mask shifted by 16 

    CVTSI2SD_XMM0_RAX = 0xc02a0f48f2,   // mov xmm0, rax
    CVTSD2SI_RAX_XMM0 = 0xc02d0f48f2,   // mov rax, xmm0

    DIVPD_XMM0_XMM1 = 0xc15e0f66,       // xmm0/xmm1 -> xmm0
    MULPD_XMM0_XMM1 = 0xc1590f66,       // xmm0*xmm1 -> xmm0

    CVTSI2SD_XMM1_RAX = 0xc82a0f48f2,   // mov xmm1, rax

    x86_RET = 0xc3,     

    x86_JMP = 0xe9,

    x86_COND_JMP = 0x000f,              // with cond mask shifted by 8

    x86_CALL = 0xe8,

    CMP_RAX_RBX = 0xd83948,

    SQRTPD_XMM0_XMM0 = 0xc0510f66,      // calculates sqrt it xmm0 

    LEA_RDI_RSP = 0x00247C8D48,         // lea rdi, [rsp]
    PUSH_ALL = 0x505152535241,          // push r10 - rax - ... - rdx
    POP_ALL  = 0x5A415B5A5958,          // pop rdx - ... rax - r10

    MOV_RBP_RSP = 0xE48949,
    MOV_RSP_RBP = 0xE4894C,
    AND_RSP_FF = 0xF0E48348,

    MOV_XMM_RSP = 0x002400100FF2,       // movsd xmm0-4, [rsp]
};

enum x86_Commands_Size
{
    SIZE_MOV_REG_IMM = 2,
    SIZE_MOV_REG_REG = 3,
    SIZE_MOV_REG_R15_OFFSET = 3,
    SIZE_MOV_R15_OFFSET_REG = 3,
    SIZE_PUSH_REG = 1,
    SIZE_PUSH_R15_OFFSET = 3,
    SIZE_POP_REG = 1,
    SIZE_POP_R15_OFFSET = 3,
    SIZE_ADD_REG_REG = 3,
    SIZE_SUB_REG_REG = 3,
    SIZE_IMUL_REG_REG = 4,
    SIZE_CQO = 2,
    SIZE_IDIV_REG = 3,
    SIZE_CVTSI2SD_XMM0_RAX = 5,
    SIZE_CVTSD2SI_RAX_XMM0 = 5,
    SIZE_DIVPD_XMM0_XMM1 = 4,
    SIZE_MULPD_XMM0_XMM1 = 4,
    SIZE_CVTSI2SD_XMM1_RAX = 5,
    SIZE_x86_RET = 1,
    SIZE_x86_JMP = 1,
    SIZE_x86_COND_JMP = 2,
    SIZE_x86_CALL = 1,
    SIZE_CMP_RAX_RBX = 3,
    SIZE_SQRTPD_XMM0_XMM0 = 4,
    SIZE_LEA_RDI_RSP = 5,
    SIZE_PUSH_ALL = 6,
    SIZE_POP_ALL = 6,
    SIZE_MOV_RBP_RSP = 3,
    SIZE_MOV_RSP_RBP = 3,
    SIZE_AND_RSP_FF = 4,
    SIZE_MOV_XMM_RSP = 6,
    SIZE_IMM = 8,
    SIZE_ADDR = 4,
};

enum X86_CMD_NAMES
{
    CMD_PUSH = 'hsup',
    CMD_POP  = 'pop',
    CMD_MOV  = 'vom',
    CMD_IN   = 'NI', //oh fuck
    CMD_OUT  = 'TUO', //oh fuck
    CMD_RET  = 'ter',
    CMD_ADD  = 'dda',
    CMD_SUB  = 'bus',
    CMD_MUL  = 'lum',
    CMD_DIV  = 'vid',
    CMD_DEG  = 'GED', //oh fuck
    CMD_JMP  = 'pmj',
    CMD_JB   = 'bj',
    CMD_JA   = 'aj',
    CMD_JBE  = 'ebj',
    CMD_JAE  = 'eaj',
    CMD_JE   = 'ej',
    CMD_JNE  = 'enj',
    CMD_SQRT = 'TRQS', //oh fuck
    CMD_IMM  = 'mmi',
    CMD_CVT  = 'tvc',
    CMD_DIVXMM = 'xvid',
    CMD_MULXMM = 'xlum',
    CMD_CMP  = 'pmc',
};

enum COND_JMPS : u_int64_t {
    JE_MASK = 0x84,
    JNE_MASK = 0x85,
    JG_MASK = 0x8f,
    JGE_MASK = 0x8d,
    JL_MASK = 0x8c,
    JLE_MASK = 0x8e,
};