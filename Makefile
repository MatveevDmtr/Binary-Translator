# Переменная CC указывает компилятор, используемый для сборки
CC=g++
#В переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -fno-omit-frame-pointer -std=c++14 -Wno-multichar -Wno-unused-result
OPTFLAG=-O2
MY_OPT=

PROCESSOR_OBJ_FILES     = Stack_main.o Assembler.o cpu.o GetPoison.o specificators.o
BINTRANSLATOR_OBJ_FILES = Parser.o Executor.o
COMMON_OBJ_FILES        = logging.o main.o

all: build execute clear

leakcheck: build use_valgrind clear

build: $(PROCESSOR_OBJ_FILES) $(BINTRANSLATOR_OBJ_FILES) $(COMMON_OBJ_FILES)
	$(CC)  $(PROCESSOR_OBJ_FILES) $(BINTRANSLATOR_OBJ_FILES) $(COMMON_OBJ_FILES) -o main

# Processor
Stack_main.o: src/Processor/Stack/Stack_main.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor/Stack/Stack_main.cpp

Assembler.o: src/Processor/Assembler/Assembler.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor/Assembler/Assembler.cpp

cpu.o: src/Processor/cpu/cpu.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor/cpu/cpu.cpp

GetPoison.o: src/Processor/GetPoison/GetPoison.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor/GetPoison/GetPoison.cpp

specificators.o: src/Processor/specificators/specificators.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Processor/specificators/specificators.cpp

# Common
logging.o: src/Common/logging/logging.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Common/logging/logging.cpp
main.o: src/Common/Main/main.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/Common/Main/main.cpp

# Binary Translator
Parser.o: src/BinTranslator/Parser/Parser.cpp
	$(CC) $(CFLAGS) $(OPTFLAG) $(MY_OPT) src/BinTranslator/Parser/Parser.cpp

Executor.o: src/BinTranslator/Executor/Executor.cpp
	$(CC) $(CFLAGS) $(MY_OPT) src/BinTranslator/Executor/Executor.cpp

clear:
	rm *.o

execute:
	./main

use_valgrind:
	valgrind --leak-check=full ./main