#ifndef ALARMS_H_
#define ALARMS_H_

#include <time.h>

#define MAX_ALARMS 30

// Defines a simple struct type containing information about an alarm
typedef struct alarm_t
{
    time_t timestamp;
    unsigned int pid;
} alarm_t;

// Get the alarm with the specified id
alarm_t get_alarm(unsigned int id);

// Get current system time
time_t get_current_time();

// Gets the amount of currently scheduled alarms
unsigned int get_alarm_count();

// Schedules a new alarm and returns the assigned ID
unsigned int schedule_alarm(time_t timestamp);

// Cancels the alarm with the given ID
void cancel_alarm(unsigned int id);

// Shuts down all pending alarm tasks
void shut_down();

// Catches zombie processes and cleans up executed alarms
void catch_zombies(); 

#endif