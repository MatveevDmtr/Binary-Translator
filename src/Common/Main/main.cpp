#include <stdio.h>
#include <time.h>

#include "../../BinTranslator/Parser/parser.h"
#include "../../BinTranslator/Executor/Executor.h"

#include "../../Processor/Assembler/assembler.h"
#include "../../Processor/cpu/cpu.h"

const int NUM_MEASURES = 10000;

int main(int argc, char** argv)
{
    Assemble(argc, argv);

    Run();

    cmdlist_t* cmdlist = CreateCmdList();

    ExecuteASM(cmdlist);

}