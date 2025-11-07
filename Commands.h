//
// Created by Henrik Ravnborg on 2025-10-31.
//

#ifndef KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
#define KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
#include <stddef.h>

typedef void (*functionPointer)(int argc, char **argv);

typedef struct {
    const char       *name;
    functionPointer  fn;
} Command;

typedef struct {
    int haveYear, year;
    int haveMonth, month;
    int haveSpan, span;
    int haveWidth, width;
} CallOptions;

extern  void command_lookup(const char *name);
extern  void cal_command(int argc, char ** argv);
extern void printCal(CallOptions *options);


#endif //KRISTIANSTADCLAB3UNIXCALENDER_COMMANDS_H
