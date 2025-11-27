#include <stdio.h>

#include "string.h"
#define INPUT_SIZE 100
#define WELCOME_MESSAGE "Welcome, Please print ur commands now: "
#define TOO_LONG_MESSAGE "Message too long, please try a shorter message\n"
#define torsdag 4
#define januari 1
#define februari 2
#include "Commands.h"

const char *weeks[] = {
    "Su",
    "Mo",
    "Tu",
    "We",
    "Th",
    "Fr",
    "Sa"
};

 enum daysMonths{
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
void printSpaces(int amount);



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

/**
 * checks if current year is leap year
 * @param year
 * @return
 */
int isLeapYear(int year){
    int leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (leap) {
        return 1;
    }
    return 0;
}


/**
 * returns the startday of the year
 *
 * @param year
 * @return INT: startday
 */
int getstartDay(int year) {
    if (year == 1970) {
        return torsdag;
    }

    int totalDays = 0;
    if (year > 1970){
        for (int y = 1970; y < year; y++) {
            totalDays += 365 + isLeapYear(y);
        }
    }else{
        for (int y = year; y < 1970; y++){
            totalDays += 365 + isLeapYear(y);
        }
        totalDays = -totalDays;
    }

    totalDays %= 7;
    if (totalDays < 0) totalDays += 7;

    return (torsdag + totalDays) % 7;
}



/**
 *
 * basicly this func returns the start day of the month
 *
 * @param month current month u wanna get
 * @param year current year
 * @param week the week
 * @return
 */
int getStartDayMonth(int month, int year){
    int startdayyear = getstartDay(year);
    if (month == 1){
        return startdayyear;
    }
    int isLeapFlag = isLeapYear(year);
    int daysBeforeMonth = 0;
    //if month == 1, this will not run, days before jan == 0
    // count the nr of days before this month
    for (int i = 1; i < month; i++){
        //if mars, count check if feb was leap, add extra
        if (isLeapFlag && i == 2){
            daysBeforeMonth += enumArray[i - 1] + 1;
        }else{
            daysBeforeMonth += enumArray[i - 1];
        }
    }
    daysBeforeMonth %= 7;
    startdayyear = (startdayyear + daysBeforeMonth ) % 7;


    return startdayyear;
}




void printCal(CallOptions *options){
    int startMonth = options->haveMonth ? options->month : januari;
    int year = options->haveYear ? options->year : 2025;
    int span = options->haveSpan ? options->span : options->haveMonth ? 1 : 12;
    int it = 0;
    int width =  options->haveWidth ? options->width : 3;
    int baseMonth  = startMonth;
    int baseYear   = year;
    printf("\n");
    for (int i = 0; i < (span + width - 1) / width; i++){
        // hur många ska jag printa?
        //defaulta till "width" om det finns månader att spara
        int totPrintLine = span - it > width ? width : span - it;

        printSpaces(7);

        for (int a = baseMonth - 1 + it; a <  baseMonth - 1 + it + totPrintLine; a++){
            if (i == 0 && a == baseMonth - 1 + it){
                printf("%d", year++);
                printSpaces(22);
            }else{
                if (a % 12 == 0){
                    printf("%d", year++);
                    printSpaces(22);
                } else  printSpaces(26);
            }
        }
        printf("\n");

        //Printa månaders namn
        for (int x = 0; x < totPrintLine; x++){
            if (x!=0) printf("     ");
            printmonth(startMonth + it + x);
        }
        printf("\n");

        //printa veckodagarnas namn
        for (int a = 0; a < totPrintLine; a++){
            for (int b = 0; b < 7; b++) printf("%s ", weeks[b]);
            printf("     ");
        }
        printf("\n");

        //print dates for each week for each month on the current row
        for (int week = 0; week < 6; week++){
            for (int b = 0; b < totPrintLine; b++){
                int month = baseMonth - 1 + it + b;
                int currentYear  = baseYear + month / 12;
                int currentMonth = month % 12 + 1;
                printDaysWeek(currentMonth ,currentYear, week);
                printf("     ");
            }
            printf("\n");
        }
        printf("\n");
        //add nr of months printed to counter
        it += totPrintLine;
    }


}


void printDaysWeek(int month, int year, int week) {
    //månadens start dag eg "tisdag" "torsdag" osvosv
    int startday = getStartDayMonth(month, year);
    int date = 1 + (7 * week);
    if (week != 0) date -= startday;
    if (week != 0) startday = 0;

    int isleapFlag = isLeapYear(year);
    for(int i = 0; i < 7; i++){
        if (startday == i){
            //special case for feb
            int val = enumArray[month - 1]; // get nr of days in month
            if (isleapFlag && month == februari) val++; // add a one exrta for feb in leapyear
            if (date <= val){
                if (date > 9) printf("%d ", date);
                else  printf(" %d ", date);
                startday++;
                date++;
            }else printf("   ");

        }else printf("   ");
    }
}

void printSpaces(int amount){
    for (int i = 0; i < amount; i++){
        printf(" ");
    }
}

int main(int argc, char* argv[])
{
    checkCommand(argc, argv);
    return 0;
}

 /*

int main(void) {
    char input[INPUT_SIZE];

    // from some guy on stackoverflow
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
*/