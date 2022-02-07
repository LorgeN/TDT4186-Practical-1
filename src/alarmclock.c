#include <stdio.h>
#include "alarms.h"

int main(int argc, char const *argv[])
{
    printf("Hello world!\n");

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("Current local time and date: %s\n", asctime(timeinfo));

    unsigned int id = schedule_alarm(rawtime);
    printf("%d \n", id);

    return 0;
}
