#include <stdio.h>

#include "../../BinTranslator/Parser/parser.h"
#include "../../BinTranslator/Executor/Executor.h"

#include "../../Processor/Assembler/assembler.h"
#include "../../Processor/cpu/cpu.h"

int main(int argc, char** argv)
{
    Assemble(argc, argv);

    Run();

    cmdlist_t* cmdlist = CreateCmdList();

    Execute(cmdlist);

}