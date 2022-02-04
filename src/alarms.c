#include "alarms.h"

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

unsigned int schedule_alarm(time_t timestamp)
{
    // TODO
}

void cancel_alarm(unsigned int id)
{
    // TODO
}