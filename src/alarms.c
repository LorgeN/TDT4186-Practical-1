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
    else
    {
        printf("Is child of Lorgen \n");
        exit(0);
        return 0;
    }
}

unsigned int schedule_alarm(time_t timestamp)
{
    alarm_t new_alarm;
    new_alarm.timestamp = timestamp;

    // TODO: Schedule alarm and set PID

    unsigned int pid = _fork_alarm(timestamp);

    printf("Expected pid %d", pid);

    new_alarm.pid = pid;

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
    memset(&alarms + alarm_count - 1, 0, sizeof(alarm_t));
    alarm_count -= 1;
}