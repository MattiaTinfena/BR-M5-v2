#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "M5StickCPlus2.h"
#include <Arduino.h>
#include "cplus_RTC.h"

extern const unsigned char icon_ble[4608];

class Display
{
private:
    LGFX_Sprite buffer;
    String name;
    String address;
    bool negatif = true; //White text on black bg if true
    float battery_lvl = 50;

    const GFXfont* font_name = &Yellowtail_32;
    const GFXfont* font_titles = &FreeSerif24pt7b;
    const GFXfont* font_text = &FreeSerif12pt7b;
    const GFXfont* font_text_small = &FreeSerif9pt7b;
public :
    Display(M5GFX* tft, String name_remote);
    void set_address(String ext_address);
    void set_init_screen(bool do_pair);
    void set_main_menu_screen(unsigned int delay, String status, unsigned int pic_count, unsigned long time, int mode);
    float battery_level();
};

#endif