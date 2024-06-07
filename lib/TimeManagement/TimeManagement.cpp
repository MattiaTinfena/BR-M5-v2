#include "cplus_RTC.h"
#include "TimeManagement.h"

void Timer::tmr_init(){
    rtc.begin();
    rtc.clearIRQ();
    rtc.disableIRQ();
    rtc.SetTime(&reset_time);
    initial_millis = millis();
    actual_time = {0,0,0};
    initial_time = {0,0,0};
    seconds = 0;
    millisec = 0;
}
void Timer::tmr_reset(){
    rtc.SetTime(&reset_time);
    initial_millis = millis();
}
void Timer::tmr_update(){
    rtc.GetTime(&actual_time);
    if(actual_time.Seconds > last_time.Seconds){
        initial_millis = millis();
    }
    last_time.Hours = actual_time.Hours;
    last_time.Minutes = actual_time.Minutes;
    last_time.Seconds = actual_time.Seconds;

    unsigned long current_millis = millis();
    seconds = actual_time.Hours*3600+actual_time.Minutes*60+actual_time.Seconds;
    millisec = (seconds*1000) + (initial_millis - current_millis);
}
    