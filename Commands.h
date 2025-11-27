//
// Created by Henrik Ravnborg on 2025-10-31.
//

#ifndef KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
#define KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
#include <stddef.h>

typedef void (*functionPointer)(int argc, char **argv);

typedef struct {
    char       *name;
    functionPointer  fn;
} Command;

typedef struct {
    int haveYear, year;
    int haveMonth, month;
    int haveSpan, span;
    int haveWidth, width;
} CallOptions;



void command_lookup(char *name);
void cal_command(int argc, char ** argv);
void printCal(CallOptions *options);
void checkCommand(int argc, char* argv[]);

#endif //KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
