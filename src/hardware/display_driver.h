#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "../config.h"

class DisplayDriver {
public:
    DisplayDriver();
    
    bool init();
    void taskHandler();
    
    static void lvglFlushCallback(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static bool lvglTouchRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    
    TFT_eSPI& getTFT() { return tft; }
    
private:
    TFT_eSPI tft;
    bool touchInitialized;
};

#endif // DISPLAY_DRIVER_H
