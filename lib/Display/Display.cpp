#include "Display.h"
#include <Arduino.h>
#include "M5StickCPlus2.h"
#include "cplus_RTC.h"

Display::Display(M5GFX* tft, String name_remote):
    buffer(tft),
    name(name_remote)
{
    buffer.createSprite(240, 135);
    buffer.setTextColor((negatif?TFT_WHITE:TFT_BLACK));
    buffer.setCursor(0, 0);
}

void Display::set_init_screen(bool do_pair)
{
    buffer.fillRect(0, 0, 240, 135, (negatif?TFT_BLACK:TFT_WHITE));
    
    buffer.setTextSize(2);
    buffer.setFreeFont(font_name);
    buffer.drawString(name, 120 - (buffer.textWidth(name)/2.0), 20 +  font_name->glyph->height); 
    
    if(do_pair){
        buffer.setTextSize(1);
        buffer.setFreeFont(font_text);
        buffer.drawString("Pairing in progress", 120 - (buffer.textWidth("Pairing in progress")/2.0), 95); 
    }

    buffer.pushSprite(0,0);
}

void Display::set_address(String address_str)
{
    address = address_str;
    address.toUpperCase();
}

void Display::set_main_menu_screen(unsigned int delay, String status, unsigned int pic_count, unsigned long time, int mode)
{
    buffer.fillRect(0, 0, 240, 135, (negatif?TFT_BLACK:TFT_WHITE));

    buffer.setFreeFont(font_text_small);

    buffer.setTextSize(1);
    buffer.drawString(address,(buffer.textWidth(address) - buffer.textWidth((String)(int)battery_level()) - buffer.textWidth("%") -4)/4 , 5);    
    buffer.drawLine(0, 28, 240, 28, (negatif?TFT_WHITE:TFT_BLACK));

    buffer.setFreeFont(font_text);

    buffer.drawString(status, 120 - (buffer.textWidth(status)/2.0), 112);
    buffer.drawLine(0, 107, 240, 107, (negatif?TFT_WHITE:TFT_BLACK));

    buffer.drawFloat(battery_level(),0, 240 - buffer.textWidth((String)(int)battery_level()) - buffer.textWidth("%") -4 ,5); 
    buffer.drawString("%", 240- buffer.textWidth("%") - 2, 5);
    if (delay == -1){

        buffer.drawString("Press button", 120 - (buffer.textWidth("Press button")/2.0), 40 );
        buffer.drawString("-->", 220 - buffer.textWidth("-->"), 55 );
        buffer.drawString("to connect", 120 - (buffer.textWidth("to connect")/2.0), 70 );
    }
    else{
        if (mode == 0){
            // setting mode

            String mode_status ="";
            switch (delay)
            {
            case 1:
                mode_status = "Single shoot";
                break;
            case 2:
                mode_status = "Timelaps";
                break;
            case 3:
                mode_status = "Startrail";
                break;
            case 4:
                mode_status = "Bulb";
                break;
            default:
            mode_status = "";
                break;
            }

            buffer.setFreeFont(font_titles);

            buffer.drawString(mode_status, 120 - (buffer.textWidth(mode_status)/2.0), 50 );
        }

        else if (mode == 1){    // single shot 
            buffer.setFreeFont(font_text);

            buffer.drawString("Pic:",120 - (buffer.textWidth("Pic:")/2),35);

            buffer.setFreeFont(font_titles);

            buffer.drawFloat(pic_count,0,120 - (buffer.textWidth((String)(int)pic_count))/2,60); 
        } 

        else if (mode == 2) {   //Timelaps


            buffer.setFreeFont(font_text);

            buffer.drawString("Pic:",180 + 10 - (buffer.textWidth("Pic:")/2),35);

            buffer.drawString("Interval(s):", 60 + 10 - (buffer.textWidth("Interval(s):")/2), 35);

            buffer.setFreeFont(font_titles);

            buffer.drawFloat(float(delay), 0, 30 + 10, 60);
            buffer.drawFloat(pic_count,0,180 + 10 - (buffer.textWidth((String)(int)pic_count))/2,60); 
        }

        else if (mode == 3) {   // Startrail


            buffer.setFreeFont(font_text);

            buffer.drawString("Pic:",180 + 10 - (buffer.textWidth("Pic:")/2),35);

            buffer.drawString("Shutter speed:", 60 + 10 - (buffer.textWidth("Shutter speed:")/2), 35);

            buffer.setFreeFont(font_titles);

            buffer.drawFloat(float(delay), 0, 30 + 10, 60);
            buffer.drawFloat(pic_count,0,180 + 10 - (buffer.textWidth((String)(int)pic_count))/2,60); 
        }

        else if (mode == 4){    //Bulb   

             if(status != "Shooting"){
                
                buffer.setFreeFont(font_text);

                buffer.drawString("Set the interval:",120 - (buffer.textWidth("Set the interval:")/2),35);

                buffer.setFreeFont(font_titles);
                float minutes;
                float sec = modf((delay/60), &minutes);
                sec = delay - (minutes*60);
                buffer.drawFloat(minutes, 0, 90 - (buffer.textWidth((String)(int)minutes))/2 , 60);
                buffer.drawString(":",120 - (buffer.textWidth(":")/2),55);
                buffer.drawFloat(sec,0,115 + (buffer.textWidth((String)(int)10))/2,60); 
            }

            else if(status == "Shooting"){
                
                buffer.setFreeFont(font_text);

                buffer.drawString("Remaining time:",120 - (buffer.textWidth("Remaining time:")/2),35);

                buffer.setFreeFont(font_titles);
                float remining_time = delay - time;
                float remining_minutes;
                float remining_seconds = modf((remining_time/60), &remining_minutes);
                remining_seconds = remining_time - (remining_minutes*60);
                
                
                buffer.drawFloat(remining_minutes, 0, 90 - (buffer.textWidth((String)(int)remining_minutes))/2 , 60);
                buffer.drawString(":",120 - (buffer.textWidth(":")/2),55);
                buffer.drawFloat(remining_seconds,0,115 + (buffer.textWidth((String)(int)10))/2,60); 
            }
        } 
    }
    buffer.pushSprite(0,0);        
    }

float Display::battery_level(void){
    float mv = (float)StickCP2.Power.getBatteryVoltage();
    float v = mv/1000;
    if (v > 4.2) {
        v = 4.2;
    }
    float level = ((v-3.27)/(4.2-3.27))*100;
    float bl = 0.8*battery_lvl+0.2*level;

    if ((bl > (battery_lvl + 0.6)) || (bl < (battery_lvl - 0.6))) {
        battery_lvl = bl;
    }
    return roundf(battery_lvl);
}
