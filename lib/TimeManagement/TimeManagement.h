#ifndef TIMEMANEGEMENT_H_
#define TIMEMANEGEMENT_H_

#include "cplus_RTC.h"

class Timer{
    public:

    RTC_TimeTypeDef initial_time;
    RTC_TimeTypeDef actual_time;
    unsigned long seconds;
    unsigned long millisec;
    void tmr_init();
    void tmr_reset();
    void tmr_update();

    private:

    cplus_RTC rtc;
    
    RTC_TimeTypeDef reset_time = {0,0,0};
    RTC_TimeTypeDef last_time;

    unsigned long initial_millis;
};

#endif