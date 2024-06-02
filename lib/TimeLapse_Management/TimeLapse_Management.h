#ifndef TimeLapse_Management_h
#define TimeLapse_Management_h

#include <Arduino.h>
#include "TimeManagement.h"
class TimeLapse {
    public :
        //constructor
        //IN/ inter_min : lower delay between shots (in ms): requiered time to reconnect to the cam
        //IN/ trigger : function taking shot
        TimeLapse(Timer* tmr, long inter_min);
        //trigger each #Interval ms
        bool TimeLapse_Trigger();
        //increase #Interval
        void TimeLapse_incDelay();
        //decrease #Interval (> #_MIN_Interval)
        void TimeLapse_decDelay();
        void Launch();
        void Stop();
        bool Recording_OnOFF();
        long get_interval();

        unsigned int Pic_count = 0;
    private :
        Timer* rtc;
        bool timeLapse_ON;
        long Interval = 0; //ms

        void(*_func_trigger)();
        unsigned long _time_last_trigger = 0; //ms
        long _MIN_Interval; //ms
        const long _delay_increment = 1000; //ms
};

#endif
