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

// Gets the amount of currently scheduled alarms
unsigned int get_alarm_count();

// Schedules a new alarm and returns the assigned ID
unsigned int schedule_alarm(time_t timestamp);

// Cancels the alarm with the given ID
void cancel_alarm(unsigned int id);

#endif