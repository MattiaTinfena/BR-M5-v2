#include "Display.h"
#include <Arduino.h>
#include "M5StickCPlus2.h"

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

void Display::set_main_menu_screen(int delay, String status, unsigned int pic_count)
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

    buffer.drawString("Pic:",180 + 10 - (buffer.textWidth("Pic:")/2),35);

    buffer.drawString("Interval (s):", 60 + 10 - (buffer.textWidth("Interval (s):")/2), 35);

    buffer.setFreeFont(font_titles);

    buffer.drawFloat(float(delay)/1000.0, 1, 30 + 10, 60);
    buffer.drawFloat(pic_count,0,180 + 10 - (buffer.textWidth((String)(int)pic_count))/2,60); 

    buffer.pushSprite(0,0);
}

float Display::battery_level(void){
    float mv = (float)StickCP2.Power.getBatteryVoltage();
    float v = mv/1000;
    if (v > 4.2) {
        v = 4.2;
    }
    float level = ((v-3.27)/(4.2-3.27))*100;
    float bl = 0.7*battery_lvl+0.3*level;

    if ((bl > (battery_lvl + 0.5)) || (bl < (battery_lvl - 0.5))) {
        battery_lvl = bl;
    }
    return roundf(battery_lvl);
}
