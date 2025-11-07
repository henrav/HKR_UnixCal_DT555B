#include <stdio.h>
#include <stdbool.h>
#include <utime.h>
#include <time.h>
#include "Commands.h"
#include "string.h"
#include <stdlib.h>
#define INPUT_SIZE 100
#define WELCOME_MESSAGE "Welcome, Please print ur commands now: "
#define NO_INPUT_MESSAGE "No input recived\n"
#define TOO_LONG_MESSAGE "Message too long, please try a shorter message\n"
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2
#define unix_day 86400
#define torsdag 7 //nu i efterhand? varför tors == 7? varför inte bara == 0?
#define fredag  1
#define lördag  2
#define söndag  3
#define måndag  4
#define tisdag  5
#define onsdag  6
#define januari 1
#define februari 2
#define mars 3
#define april 4
#define maj 5
#define juni 6
#define juli 7
#define augusti 8
#define september 9
#define oktober 10
#define november 11
#define december 12



const char *weeks[] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

const enum daysMonths{
    jan = 31,
    feb = 28,
    mar = 31,
    apr = 30,
    may = 31,
    jun = 30,
    jul = 31,
    aug = 31,
    sep = 30,
    oct = 31,
    nov = 30,
    dec = 31
};

const int enumArray[] = {
        jan, feb, mar, apr, may, jun, jul, aug,sep,oct, nov, dec
};



void printDaysWeek(int month, int year, int week);



void printmonth(int months) {
    months = ((months - 1) % 12) + 1;
    switch (months) {
        case 1:
            printf("      januari       ");
            break;
        case 2:
            printf("      februari      ");
            break;
        case 3:
            printf("        mars        ");
            break;
        case 4:
            printf("       april        ");
            break;
        case 5:
            printf("        maj         ");
            break;
        case 6:
            printf("        juni        ");
            break;
        case 7:
            printf("        juli        ");
            break;
        case 8:
            printf("      augusti       ");
            break;
        case 9:
            printf("     september      ");
            break;
        case 10:
            printf("      oktober       ");
            break;
        case 11:
            printf("      november      ");
            break;
        case 12:
            printf("      december      ");
            break;
        default:
            printf("error");
            break;
    }

}
int isLeapYear(int year){
    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) {
        return 1;
    }
    return 0;
}

int getstartDay(int year) {
    if (year == 1970) {
        return torsdag;
    }

    int totalDays = 0;
    for (int y = 1970; y < year; y++) {
        totalDays += 365 + isLeapYear(y);
    }

    totalDays %= 7;
    return (torsdag + totalDays) % 7;
}



int getStartDayMonthPerWeek(int month, int year, int week){
    int isLeapFlag = isLeapYear(year);
    int daysBeforeMonth = 0;
    int startdayyear = getstartDay(year);
    //if month == 1, this will not run, days before jan == 0
    for (int i = 1; i < month; i++){
        //if mars, count check if feb was leap, add extra
        if (isLeapFlag && i == 2){
            daysBeforeMonth += enumArray[i - 1] + 1;
        }else{
            daysBeforeMonth += enumArray[i - 1];
        }
    }

    daysBeforeMonth %= 7;

    startdayyear = (startdayyear + daysBeforeMonth + (week * 7)) % 7;

    return startdayyear;
}



void printCenterSpaces(int width, int i) ;

void printCal(CallOptions *options){
    int startMonth = options->haveMonth ? options->month : januari;
    int year = options->haveYear ? options->year : 2025;
    int span = options->haveMonth ? 1 : options->haveSpan ? options->span : 12;
    int it = 0;
    int width =  options->haveMonth ? 1 : options->haveWidth ? options->width : 3;
    printf("\n");
    printCenterSpaces(width, 4);
    int monthCounter = 0;
    printf("%d", year);
    printf("\n");
    /*
     * Tog alldeless för lång tid att fixa till denna print function, allt annat var enkelt i jämnförelse
     *
          width == 3, nrOfMonths == 4, då ska man printa 2 rader

          jan       feb         mars  == rad 0
          april                       == rad 1

          4+3=7; 7-1=6; 6/3=2 == 2 rader


          width == 3, nrOfMonths == 3, då ska man printa 1 rad

          jan       feb         mars  == rad 0

          3+3=6; 6-1=5; 5/3=1.666 == 1 rad
     */
    for (int i = 0; i < (span + width - 1) / width; i++){

        // hur många ska jag printa?
        //defaulta till "width" om det finns månader att spara
        int totPrintLine = span - it > width ? width : span - it;


        if (monthCounter > 11){
            printCenterSpaces(width,4 );
            printf("%d", ++year);
            printf("\n");
            monthCounter = 0;
        }
        //Printa månaders namn
        for (int x = 0; x < totPrintLine; x++){
            if (x!=0){
                for (int a = x; a < x + 6; a++){
                    printf(" ");
                }
            }

            printmonth(startMonth + (it ));
            it++;
            monthCounter++;
        }
        printf("\n");


        //printa veckodagarnas namn
        for (int a = 0; a < totPrintLine; a++){
            for (int b = 0; b < 7; b++){
                printf("%s ", weeks[b]);
            }
            printf("     ");
        }
        printf("\n");

        // printa datum för alla veckor
        for (int c = 0; c < 6; c++){
            for (int b = totPrintLine; b > 0; b--){
                int currentMonth = startMonth + it - b;
                currentMonth = ((currentMonth - 1) % 12) + 1;
                printDaysWeek(currentMonth ,year, c);
                printf("     ");
            }
            printf("\n");
        }
        printf("\n");
    }


}


void printDaysWeek(int month, int year, int week) {
    //månadens start dag eg "tisdag" "torsdag" osvosv
    int startday = getStartDayMonthPerWeek(month, year, week);

    // normalisera till söndag == 0;
    // annars söndag == 3 enligt unix grejen;
    startday = (startday + 4) % 7;

    int date = 1 + (7 * week);
    if (week != 0){
        date = 1 + (7 * week) - startday;
    }
    if (week != 0){
        startday = 0;
    }


    int isleapFlag = isLeapYear(year);
    for(int i = 0; i < 7; i++){
        if (startday == i){
            //special case for feb
            int val = enumArray[month - 1];
            if (isleapFlag && month == februari) val++;
            if (date <= val){
                if (date > 9){
                    printf("%d ", date);
                }else{
                    printf(" %d ", date);
                }
                startday++;
                date++;
            }else{
                printf("   ");
            }

        }else{
            printf("   ");
        }
    }

}

void printCenterSpaces(int width, int offset){
    for (int i = 0; i < ((width * 24) / 2) - offset; i++){
        printf(" ");
    }
}


int main(void) {
    char input[INPUT_SIZE];
    for (;;) {
        fflush(stdout);
        printf("type 'help' for help for help with args\n");
        printf("type 'exit' to quit\n");
        printf(WELCOME_MESSAGE);
        int ch, extra;
        if (fgets(input, INPUT_SIZE, stdin) == NULL)
            continue;

        if (input[strlen(input) - 1] != '\n') {
            extra = 0;
            while (((ch = getchar()) != '\n') && (ch != EOF))
                extra = 1;
            if (extra == 1) {
                printf(TOO_LONG_MESSAGE);
                continue;
            }
        }
        input[strlen(input) - 1] = '\0';
        command_lookup(input);

    }
}
