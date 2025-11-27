//
// Created by Henrik Ravnborg on 2025-10-31.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Commands.h"
#include <ctype.h>

void printHelpCal(int argc, char **argv){
    printf("valid arguments for command 'mycal' are: \n");
    printf("\n");
    printf("-m [month]: \n");
    printf("\tUSAGE: '-m [1-12]'\n");
    printf("\tEXAMPLE: 'mycal -m 3'\n");
    printf("\tRESULT: above example will print calendar starting from mars\n");
    printf("\tNOTES: ");
    printf("\tEnables selection of month to print. '-m 1' = january, '-m 12' = december\n");
    printf("\tWhen not specified, 'mycal' will print all 12 months of the year unless the arg '-s' is used");
    printf("\n\n");
    printf("-y [year]: \n");
    printf("\tUSAGE: 'mycal -y [0-9999]' \n");
    printf("\tEXAMPLE: 'mycal -y 2015'\n");
    printf("\tRESULT: above example will print calendar for year 2015\n");
    printf("\tNOTES: ");
    printf("\tEnables selection of year to print. 'cal -y 2025' prints cal for 2025\n");
    printf("\tWhen not specified, 'mycal' will print the year 2025");
    printf("\n\n");
    printf("-s [span]: \n");
    printf("\tUSAGE: 'mycal -s [1-9999]' \n");
    printf("\tEXAMPLE: 'mycal -s 50'\n");
    printf("\tRESULT: above example will print calendar spanning 50 months into the future\n");
    printf("\tNOTES: ");
    printf("\tSpan of months, eg: 'mycal -s 50' = print 50 months into the future.\n");
    printf("\tWhen not specified, the span of 'mycal' will be either 1 or 12 depending on if the '-m' arg is used");
    printf("\n\n");
    printf("-w [width]: \n");
    printf("\tUSAGE: 'mycal -w [1-9999]' \n");
    printf("\tEXAMPLE: 'mycal -w 6'\n");
    printf("\tRESULT: above example will print calendar with each row containing 6 months\n");
    printf("\tNOTES: ");
    printf("\tEnables selection of how many months will be printed on each row\n");
    printf("\tExample: 'mycal -w 12' will print:\n");
    printf("\tJAN      FEB      MAR      APR      MAY      ...      ...\n");
    printf("\n");
    printf("\tExample: 'mycal -w 1' will print:\n");
    printf("\t\t\tJAN\n");
    printf("\t\t\tFEB\n");
    printf("\t\t\tMAR\n");
    printf("\t\t\tAPR\n");
    printf("\t\t\tMAY\n");
    printf("\t\t\t...\n");
    printf("\t\t\t...\n");
    printf("\n");
    printf("GENERAL NOTES: combination of arguments are possible and they are not positional.\n");
    printf("EXAMPLE OF VALID COMMANDS: \n ");
    printf("\t mycal -y 2015 -m 4  :  this will print year 2015 starting from month 4 (april)\n ");
    printf("\t mycal -m 4 -y 2015 -s 100  :  this will print year 2015 starting from month 4 (april) spanning 100 months into the future\n ");
    printf("\t mycal -m 4 -y 2015 -s 100 -w 5  :  this will print year 2015 starting from month 4 (april) spanning 100 months into the future with 5 months per row\n ");
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
            if (*end || year < 1 || year > 9999) {
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
            if (*end || span < 1 || span > 9999) {
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
            // didnt have time to come up with another solutions that didnt involve sending unnecessary args
            printHelpCal(argc,argv);
            return 0;
        } else {
            printf("Unexpected argument: %s\n", argv[i]);
            // didnt have time  to come up with another solutions that didnt involve sending unnecessary args
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
Command commandTable[]={
        {
            "mycal",
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



void checkCommand(int argc, char* argv[]){


    if (argc > 1){
        if (argv[1][0] != '-'){
            for (int i = 0; i < cmdTableSize; i++) {
                if (commandTable[i].name && strcmp(argv[1], commandTable[i].name) == 0) {
                    commandTable[i].fn(argc - 2, &argv[2]);
                    return;
                }
            }
            fprintf(stderr, "Unknown command: %s\n", argv[1]);
            return;
        }else{
            commandTable[0].fn(argc - 1, &argv[1]);
            return;
        }
    }
    commandTable[0].fn(0, NULL);
    return;
}
/*

void command_lookup(char *command){
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
*/

