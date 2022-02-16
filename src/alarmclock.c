#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alarms.h"

//Validate that the users alarm to schedule is correct
int validate_time(time_t users_time) {
    double res = difftime(users_time, get_current_time());

    

    if (res < 0) {
        printf("You did not enter a valid time. You cannot schedule an alarm back in time!");
        return 0;
    }

    return 1;
}



int menu() {

    char choice[100];
    printf("Please enter \"s\" for schedule a new alarm\n");
    printf("Please enter \"l\" for see list of active alarms\n");
    printf("Please enter \"c\" for canceling a scheduled alarm\n");
    printf("Please enter \"x\" for exit the program\n");
    printf("> ");
    scanf("%s", choice);

    if (strcmp(choice, "s") == 0) {

        printf("enter an integer: ");
        //scanf("[^n]%ld", );
        
    }
    else if (strcmp(choice, "l") == 0) {
        printf("Active alarms: \n");
        for (int i = 0; i < get_alarm_count(); i++)
        {
            alarm_t current_alarm = get_alarm(i);
            time_t tid = current_alarm.timestamp;
            struct tm *converted_time;
            converted_time = localtime(&tid);
            printf("Alarm %d at %s\n", i+1, asctime(converted_time));
        }
        
    }
    else if (strcmp(choice, "c") == 0) {
        unsigned int id;
        scanf("%u Cancel which alarm? ", &id);
        cancel_alarm(id);
    }
    else if (strcmp(choice, "x") == 0) {
        printf("See you later\n");
        exit(0);
    }
    else {
        printf("%s is not a valid option, choose one of the valid inputs\n", choice);
        scanf("%s", choice);
    }
    
    return 0;
}

int main(int argc, char const *argv[])
{
    printf("Welcome to the alarm clock! it is currently: ");

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("%s\n", asctime(timeinfo));

    schedule_alarm(rawtime + 40);
    schedule_alarm(rawtime + 50);
    schedule_alarm(rawtime + 55);

    menu();
    
    
    
    
    

    return 0;
}
