#include "alarms.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int alarm_count = 0;
alarm_t alarms[MAX_ALARMS];

alarm_t get_alarm(unsigned int id)
{
    return alarms[id];
}

unsigned int get_alarm_count()
{
    return alarm_count;
}

unsigned int _fork_alarm(time_t timestamp)
{
    unsigned int pid = fork();

    if (pid != 0)
    {
        printf("Is default \n");
        return pid;
    }

    printf("Is child of Lorgen \n");
    exit(0);
    return 0;
}

unsigned int schedule_alarm(time_t timestamp)
{
    // Avoid modifying array before everything is set up
    unsigned int pid = _fork_alarm(timestamp);

    alarm_t new_alarm;
    new_alarm.timestamp = timestamp;
    new_alarm.pid = pid;

    // Alarm count is the total amount of alarms we have so naturally
    // if we use alarm count as id (and index) it will be added after
    // the last current alarm in the array
    unsigned int alarm_id = alarm_count;
    alarms[alarm_id] = new_alarm;
    alarm_count += 1;

    return alarm_id;
}

void cancel_alarm(unsigned int id)
{
    alarm_t alarm = get_alarm(id);
    kill(alarm.pid, SIGKILL);

    alarm_t *dest = alarms + id;
    memmove(dest, dest + 1, sizeof(alarm_t) * (alarm_count - id - 1));
    memset(alarms + alarm_count - 1, 0, sizeof(alarm_t));
    alarm_count -= 1;
}