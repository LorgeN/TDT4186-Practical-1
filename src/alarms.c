#include "alarms.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

unsigned int alarm_count = 0;
alarm_t alarms[MAX_ALARMS];

void __clean_up_alarm(unsigned int id)
{
    alarm_t *dest = alarms + id;
    memmove(dest, dest + 1, sizeof(alarm_t) * (alarm_count - id - 1));
    memset(alarms + alarm_count - 1, 0, sizeof(alarm_t));
    alarm_count -= 1;
}

alarm_t get_alarm(unsigned int id)
{
    return alarms[id];
}

unsigned int get_alarm_count()
{
    return alarm_count;
}

time_t get_current_time()
{
    time_t rawtime;
    time(&rawtime);
    return rawtime;
}

void _ring_alarm()
{
    printf("RING! Alarm ended!\n");
#ifdef _WIN32
    // We don't do much to support Windows
    printf("You use Windows so you don't get any cool sounds :'(")
#elif __APPLE__
    execlp("afplay", "afplay", "./alarm.mp3", 0);
#elif __unix__
    execlp("mpg123", "mpg123", "-q", "./alarm.mp3", 0);
#endif
}

unsigned int _fork_alarm(time_t timestamp)
{
    unsigned int pid = fork();

    if (pid != 0)
    { // if its the parent main proccess
        return pid;
    }
    else
    { // If its a child process

        time_t current_time = get_current_time();

        int seconds_to_wait = (int)difftime(timestamp, current_time);

        if (seconds_to_wait < 0)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            sleep(seconds_to_wait);
            _ring_alarm();
            exit(EXIT_SUCCESS);
        }
    }
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

void catch_zombies()
{
    pid_t pid = waitpid(-1, NULL, WNOHANG);

    // If no child has terminated (pid_t) 0 is returned
    if (pid == (pid_t)0)
    {
        return;
    }

    unsigned int id = -1;
    for (int i = 0; i < get_alarm_count(); i++)
    {
        if ((pid_t)get_alarm(i).pid == pid)
        {
            id = i;
            break;
        }
    }

    if (id == -1)
    {
        return;
    }

    __clean_up_alarm(id);
}

void cancel_alarm(unsigned int id)
{
    alarm_t alarm = get_alarm(id);
    kill(alarm.pid, SIGKILL);
    __clean_up_alarm(id);
}

void shut_down()
{
    alarm_t current;
    for (int i = 0; i < get_alarm_count(); i++)
    {
        current = get_alarm(i);
        kill(current.pid, SIGKILL);
    }
}