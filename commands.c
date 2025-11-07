//
// Created by Henrik Ravnborg on 2025-10-31.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include <ctype.h>

void printHelpCal(int argc, char **argv){
    printf("valid arguments for command 'cal' are: \n");
    printf("\n");
    printf("-m [month]: \n");
    printf("\tUSAGE: '-m [1-12]'\n");
    printf("\tNOTES: ");
    printf("\tEnables selection of month to print. '-m 1' = january, '-m 12' = december\n");
    printf("\tUsage of this arg will nullify arg '-w' and '-s'\n");
    printf("\tWhen not specified, 'cal' will print all 12 months of the year unless the arg '-s' is used");
    printf("\n\n");
    printf("-y [year]: \n");
    printf("\tUSAGE: 'cal -y [0-9999]' \n");
    printf("\tNOTES: ");
    printf("\tEnables selection of year to print. 'cal -y 2025' prints cal for 2025\n");
    printf("\tWhen not specified, 'cal' will print the year 2025");
    printf("\n\n");
    printf("-s [span]: \n");
    printf("\tUSAGE: 'cal -s [1-9999]' \n");
    printf("\tNOTES: ");
    printf("\tSpan of months, eg: 'cal -s 50' = print 50 months into the future. when combined with '-m', this arg wont do anything\n");
    printf("\tWhen not specified, the span of 'cal' will be either 1 or 12 depending on if the '-m' arg is used");
    printf("\n\n");
    printf("-w [width]: \n");
    printf("\tUSAGE: 'cal -w [1-9999]' \n");
    printf("\tNOTES: ");
    printf("\tEnables selection of how many months will be printed on each row\n");
    printf("\tExample: 'cal -w 12' will print:\n");
    printf("\tJAN      FEB      MAR      APR      MAY      ...      ...\n");
    printf("\n");
    printf("\tExample: 'cal -w 1' will print:\n");
    printf("\t\t\tJAN\n");
    printf("\t\t\tFEB\n");
    printf("\t\t\tMAR\n");
    printf("\t\t\tAPR\n");
    printf("\t\t\tMAY\n");
    printf("\t\t\t...\n");
    printf("\t\t\t...\n");
    printf("\n");   printf("\n");

    printf("\n");   printf("\n");

}


static int parse_cal_flags(int argc, char **argv, CallOptions *options) {
    memset(options, 0, sizeof *options);
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-y") == 0) {
            if (i + 1 >= argc) {
                printf("missing value after -y \n");
                return 0;
            }
            char *end;
            long year = strtol(argv[++i], &end, 10);
            if (*end) {
                printf("year must be a number \n");
                return 0;
            }
            options->haveYear = 1;
            options->year = (int) year;
        } else if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 >= argc) {
                printf("missing value after -m \n");
                return 0;
            }
            char *end;
            long month = strtol(argv[++i], &end, 10);
            if (*end || month < 1 || month > 12) {
                printf("month must be 1..12 \n");
                return 0;
            }
            options->haveMonth = 1;
            options->month = (int) month;
        } else if (strcmp(argv[i], "-s") == 0){
            if (i + 1 >= argc) {
                printf("missing value after -s \n");
                return 0;
            }
            char *end;
            long span = strtol(argv[++i], &end, 10);
            if (*end) {
                printf("span must be a number \n");
                return 0;
            }
            options->haveSpan = 1;
            options->span = (int) span;
        }else if (strcmp(argv[i], "-w") == 0){
            if (i + 1 >= argc) {
                printf("missing value after -w \n");
                return 0;
            }
            char *end;
            long width = strtol(argv[++i], &end, 10);
            if (*end || width < 1) {
                printf("width must be a positive number \n");
                return 0;
            }
            options->haveWidth = 1;
            options->width = (int) width;
        }else if (argv[i][0] == '-') {
            printf("unknown flag: %s\n", argv[i]);
            // didnt have time or energy to come up with another solutions that didnt involve sending uncessicary variable names
            printHelpCal(argc,argv);
            return 0;
        } else {
            printf("Unexpected argument: %s\n", argv[i]);
            // didnt have time or energy to come up with another solutions that didnt involve sending uncessicary variable names
            printHelpCal(argc,argv);
            return 0;
        }
    }
    return 1;
}

void cal_command(int argc, char **argv) {
    CallOptions o;
    if (parse_cal_flags(argc, argv, &o)) printCal(&o);
}


void command_exit(int argc, char **argv)
{
    exit(0);
}
const Command commandTable[]={
        {
            "cal",
            cal_command
        },{
            "exit",
                command_exit
        },
        {
            "help",
            printHelpCal
        }
};


const int cmdTableSize = sizeof(commandTable) / sizeof(commandTable[0]);
int tokenizeInput(char *command, char **argv, int maxLength){
    int nrOfArgs = 0;
    char *pointer = command;
    while (*pointer && isspace(*pointer)) pointer++;

    while (*pointer && nrOfArgs < maxLength - 1){
        argv[nrOfArgs++] = pointer;
        while (*pointer && !isspace(*pointer)){
            pointer++;
        }
        if (!*pointer) break;
        *pointer++ = '\0';
        while (*pointer && isspace((unsigned char)*pointer)) pointer++;
    }
    argv[nrOfArgs] = NULL;
    return nrOfArgs;

}



const Command* checkCommand(const char *string){
    for (int i = 0; i < cmdTableSize; i++ ){
        if (commandTable[i].name && strcmp(string, commandTable[i].name) == 0){
            return &commandTable[i];
        }
    }
    printf("no command found for command: %s\n", string);
    printf("DID U MEAN 'cal', 'exit' ? \n");
    printf("\n");
    return NULL;
}

void command_lookup(const char *command){
    char buf[256];
    strncpy(buf, command, sizeof buf - 1);
    buf[sizeof buf - 1] = '\0';
    char *argv[100];
    int argc = tokenizeInput(buf, argv, 100);
    if (argc == 0){
        printf("no command, please write real command\n");
        return;
    }

    int argCount = argc - 1;
    const Command *cmd = checkCommand(argv[0]);
    if (!cmd) return;
    char **args = &argv[1];
    cmd->fn(argCount, args);

}

