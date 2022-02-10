#include <stdio.h>
#include "alarms.h"

int menu() {
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Extended version with NFTS comming soon!\n");

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s\n", asctime(timeinfo));

    char choice;
    printf("Please enter \"s\" for schedule a new alarm\n");
    printf("Please enter \"l\" for see list of active alarms\n");
    printf("Please enter \"c\" for canceling a scheduled alarm\n");
    printf("Please enter \"x\" for exit the program\n");
    scanf("%s", &choice);

    unsigned int id = schedule_alarm(rawtime);
    printf("%d \n", id);

    return 0;
}
