#include "CanonBLERemote.h"
#include <Arduino.h>
#include "Ticker.h"
#include "M5StickCPlus2.h"
#include "Display.h"
#include "TimeManagement.h"

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include <esp32-hal-log.h>

String name_remote = "BR-M5";
CanonBLERemote canon_ble(name_remote);
Display M5_display(&M5.Lcd, name_remote);
String status;
Timer rtc;

unsigned int pic_count = 0;
int interval = 0;
bool shooting = false;
bool mode_init = false;

enum Mode {     
                settings_mode, 
                single_shot_mode,
                timelapse_mode,
                startrail_mode,
                bulb_mode

            } current_mode;

void setup()
{
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_INFO); 

    M5.begin();
    bool do_pair = M5.BtnA.isPressed();
    M5.Lcd.setRotation(1);
    M5_display.set_init_screen(do_pair);
    
    // To pair a new camera press A button while M5Stick starts up

    current_mode = settings_mode;

    canon_ble.init();
    
    delay(500);
    // Pairing
    if(do_pair){
        // pair() function should be called only when you want to pair with the new camera. 
        // After paired, the pair() function should not be called.
        do{
            Serial.println("Pairing...");
        }
        while(!canon_ble.pair(10));

    }

    delay(500);
    Serial.println("Setup Done");
    
    M5_display.set_address(canon_ble.getPairedAddressString());
    rtc.tmr_init();

    status = "Choose the mode";
    M5_display.set_main_menu_screen(interval, status, pic_count, rtc.seconds, current_mode);
}

void loop()
{
    // Update buttons state
    M5.update();
    // Update rtc 
    rtc.tmr_update();
    
    if ((current_mode != settings_mode) && !canon_ble.isConnected()){
        //checking if camera is connected
        status = "Camera disconnected";
        interval = -1;  
        if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased() && !M5.BtnPWR.wasReleased()){
            canon_ble.focus();
        }
        else if (M5.BtnB.wasReleasefor(700))
        {
            status = "Choose the mode";
            current_mode = settings_mode;
        }
    }
    else{
        switch (current_mode)
        {
            case settings_mode:
            {
                // using the interval field as selected_state
                unsigned int selected_state = 1;
                while(current_mode == settings_mode)
                {
                    M5.update();
                    rtc.tmr_update();
                    M5_display.set_main_menu_screen(selected_state, status, pic_count, rtc.seconds, current_mode);
                    
                    if (M5.BtnB.wasReleased())
                    {
                        selected_state++;
                        if (selected_state > 4)
                        {
                            selected_state = 1;
                        }
                    }
                    else if (M5.BtnPWR.wasReleased())
                    {
                        selected_state--;
                        if (selected_state < 1)
                        {
                            selected_state = 4;
                        }
                    }
                    else if (M5.BtnA.wasReleased())
                    {
                        mode_init = true;
                        current_mode = static_cast<Mode>(selected_state);
                    }

                    delay(20);
                }
                break;
            }

            case single_shot_mode:
            {
                //
                if(mode_init){
                    status = "Ready for single shot";
                    interval = 0;
                    mode_init = false;
                }
                else{
                    if (M5.BtnA.wasReleased() && !M5.BtnB.wasReleased() && !M5.BtnPWR.wasReleased())
                    {
                        // Press the A button to take a single photo
                        if(!canon_ble.trigger())
                        {
                            status = "Trigger Failed";
                        }
                        pic_count++;
                    }
                    else if (M5.BtnB.wasReleasefor(700))
                    {
                        //Hold B button to main menu
                        pic_count = 0;
                        status = "Choose the mode";
                        current_mode = settings_mode;
                    }
                }
                break;
            }

        case timelapse_mode:
            {
                //Timelapse mode shoot a photo each "interval" sec. shutter time must be lower than interval 
                if (mode_init){
                    status = "Ready for timelapse";
                    interval = 0;
                    mode_init = false;
                }
                else{
                    //Hold B button to main menu
                    if (M5.BtnB.wasReleasefor(700) && !shooting){
                       
                        interval = 0;
                        pic_count = 0;
                        shooting = false;
                        status = "Choose the mode";
                        current_mode = settings_mode;
                    }
                    else{
                        if (M5.BtnB.wasReleased() && !shooting){
                            // Press the B button to increase the interval
                            if (interval <= 0){
                                interval = 1;
                            }
                            else {
                                interval += 1;
                            }
                        }
                        else if (M5.BtnPWR.wasReleased() && !shooting){
                            // Press the POWER button to decrease the interval
                            if (interval <= 1){
                                interval = 0;
                            }
                            else {
                                interval -= 1;
                            }
                        }
                        else if (M5.BtnA.wasReleased() && !shooting){
                           // Press the A button to start the timelapse
                            status = "Shooting";
                            shooting = true;
                            rtc.tmr_reset();
                            if(!canon_ble.trigger())
                            {
                                status = "Trigger Failed";
                            }
                        }
                    
                        else if (shooting){
                            if(rtc.seconds > interval){
                                if(!canon_ble.trigger()){
                                    status = "Trigger Failed";
                                } 
                                pic_count++;
                                rtc.tmr_reset();
                            }
                            if (M5.BtnA.wasReleased()){
                                // Press the A button to stop the timelapse
                                status = "Ready for timelapse";
                                shooting = false;
                                pic_count = 0;
                            }
                        }
                    }
                }
                break;
            }

            case startrail_mode:
            {
                // Startrail mode shoot photo continuously to do this the user have to specify the shutter speed,
                // the BR-M5 will shoot a photo each "shutter speed" sec + 10%

                // NOTE: the 10% value is an empirical value obtained using an M50 mkI with a lexar silver 64 GB SD
                // Make sure to test this value before use the startrail mode 
                if (mode_init){
                    status = "Ready for startrail";
                    interval = 0;
                    mode_init = false;
                }
                else{
                    if (M5.BtnB.wasReleasefor(700) && !shooting){
                       //Hold B button to main menu
                        interval = 0;
                        pic_count = 0;
                        shooting = false;
                        status = "Choose the mode";
                        current_mode = settings_mode;
                    }
                    else{
                        if (M5.BtnB.wasReleased() && !shooting){
                            // Press the B button to increase the shutter speed
                            if (interval <= 0){
                                interval = 1;
                            }
                            else {
                                interval += 1;
                            }
                        }
                        else if (M5.BtnPWR.wasReleased() && !shooting){
                            // Press the PWR button to decrease the shutter speed
                            if (interval <= 1){
                                interval = 0;
                            }
                            else {
                                interval -= 1;
                            }
                        }
                        else if (M5.BtnA.wasReleased() && !shooting){
                            // Press the A button to start the startrail
                            status = "Shooting";
                            shooting = true;
                            rtc.tmr_reset();
                            if(!canon_ble.trigger())
                            {
                                status = "Trigger Failed";
                            }
                        }
                    
                        else if (shooting){
                            if(rtc.seconds > (((float)interval)*1.1)){ // *1.1 means an increase of 10% of the shutter speed value 
                                if(!canon_ble.trigger()){
                                status = "Trigger Failed";
                                } 
                                pic_count++;
                                rtc.tmr_reset();
                            }
                            if (M5.BtnA.wasReleased()){
                                // Press the A button to stop the timelapse
                                status = "Ready for startrail";
                                shooting = false;
                                pic_count = 0;
                            }
                        }
                    }
                }
                break;
            }

            case bulb_mode:
            {
                // Bulb mode shoot a photo with the shutter speed chosen by the user
                if (mode_init){
                    status = "Ready for bulb shoot";
                    interval = 0;
                    mode_init = false;
                }
                else{
                    if (M5.BtnB.wasReleasefor(700) && !shooting){
                       //Hold B button to main menu
                        interval = 0;
                        shooting = false;
                        status = "Choose the mode";
                        current_mode = settings_mode;
                    }
                    else{
                        if (M5.BtnB.wasReleased() && !shooting){
                            // Press the B button to increase the shutter speed by 10 sec
                            if (interval <= 0){
                                interval = 10;
                            }
                            else {
                                interval += 10;
                            }
                        }
                        else if (M5.BtnPWR.wasReleased() && !shooting){
                            // Press the PWR button to decrease the shutter speed by 10 sec
                            if (interval <= 10){
                                interval = 0;
                            }
                            else {
                                interval -= 10;
                            }
                        }
                        else if (M5.BtnA.wasReleased() && !shooting){
                            // Press the A button to start the bulb
                            status = "Shooting";
                            shooting = true;
                            rtc.tmr_reset();
                            if(!canon_ble.trigger()){
                                status = "Trigger Failed";
                            }
                        }
                    
                        else if (shooting){
                            if(rtc.seconds > interval){
                                if(!canon_ble.trigger()){
                                status = "Trigger Failed";
                                } 
                                status = "Ready for bulb shoot";
                                shooting = false;
                            }
                        }
                    }
                }
                break;
            }

        }
    }
    
    M5_display.set_main_menu_screen(interval, status, pic_count, rtc.seconds, current_mode);
    delay(20);
}
