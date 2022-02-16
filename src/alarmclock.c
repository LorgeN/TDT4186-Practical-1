#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "alarms.h"

char *__format_time(time_t *time)
{
    struct tm *formatted;
    formatted = localtime(time);
    return asctime(formatted);
}

// "Inspired" by https://cboard.cprogramming.com/c-programming/169114-how-convert-string-time_t.html
// Returns false if input is a valid date, true otherwise
bool __read_time(time_t *time)
{
    time_t result;

    int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

    if (scanf("%4d.%2d.%2d %2d:%2d:%2d", &year, &month, &day, &hour, &min, &sec) != 6)
    {
        fprintf(stderr, "The input was not a valid time format\n");
        return true;
    }

    struct tm breakdown = {0};
    breakdown.tm_year = year - 1900; // From man page we see that year starts at 1900
    breakdown.tm_mon = month - 1;    // Months start at 0 (January)
    breakdown.tm_mday = day;
    breakdown.tm_hour = hour;
    breakdown.tm_min = min;
    breakdown.tm_sec = sec;

    if ((result = mktime(&breakdown)) == (time_t)-1)
    {
        fprintf(stderr, "Could not convert time input to time_t\n");
        return true;
    }

    *time = result;
    return false;
}

int menu()
{
    char choice[100];
    printf("Please enter \"s\" for schedule a new alarm\n");
    printf("Please enter \"l\" for see list of active alarms\n");
    printf("Please enter \"c\" for canceling a scheduled alarm\n");
    printf("Please enter \"x\" for exit the program\n");
    printf("> ");
    scanf("%s", choice);

    if (strlen(choice) != 1)
    {
        printf("Invalid input!\n");
        return 1;
    }

    // Run this here aswell to make sure we get rid of any completed alarms before
    // showing to the user
    catch_zombies();

    unsigned int id;
    bool cancel = false;
    switch (choice[0])
    {
    case 's':

        printf("Please enter a date on the format YYYY.MM.DD HH:MM:SS\n> ");
        time_t time;
        if (__read_time(&time))
        {
            return 1;
        }

        int time_difference = (int)difftime(time, get_current_time());
        if (time_difference < 0)
        {
            printf("Can't schedule an alarm for a time that has already passed! Time ago: %d\n", time_difference);
            return 1;
        }

        id = schedule_alarm(time);
        printf("Scheduled alarm %u at %s\n(In %d second(s))\n", id + 1, __format_time(&time), time_difference);
        return 1;
    case 'c':
        cancel = true;
    case 'l':
        if (!get_alarm_count())
        {
            printf("No scheduled alarms!\n");
            return 1;
        }

        printf("Active alarms: \n");
        for (int i = 0; i < get_alarm_count(); i++)
        {
            alarm_t current_alarm = get_alarm(i);
            printf("Alarm %d at %s\n", i + 1, __format_time(&current_alarm.timestamp));
        }

        if (!cancel)
        {
            return 1;
        }

        printf("Enter the ID of the alarm to cancel\n> ");
        scanf("%u", &id);
        if (id > get_alarm_count())
        {
            printf("Unknown alarm %u!", id);
            return 1;
        }

        alarm_t target = get_alarm(id - 1);
        cancel_alarm(id - 1);
        printf("Cancelled alarm %u at %s\n", id, __format_time(&target.timestamp));
        return 1;
    case 'x':
        printf("See you later\n");
        return 0;
    default:
        printf("%s is not a valid option, choose one of the valid inputs\n", choice);
        return 1;
    }

    return 1;
}

int main(int argc, char const *argv[])
{
    time_t current_time = get_current_time();
    printf("Welcome to the alarm clock! It is currently %s\n", __format_time(&current_time));

    while (menu())
    {
        catch_zombies();
    }

    // Kill all pending alarm tasks so they dont suddenly play when done!
    shut_down();
    return EXIT_SUCCESS;
}
