#ifndef SCREEN_ACTIONS_H
#define SCREEN_ACTIONS_H

#include "screen_base.h"

class ScreenActions : public ScreenBase {
public:
    ScreenActions();
    void load() override;
    
private:
    // Statusbar elements
    lv_obj_t* statusbar;
    lv_obj_t* lblExtruderTemp;
    lv_obj_t* lblBedTemp;
    lv_obj_t* lblTitle;
    lv_obj_t* lblTime;
    
    // Grid container
    lv_obj_t* buttonGrid;
    
    // Button pointers (for dynamic updates if needed)
    lv_obj_t* btnMove;
    lv_obj_t* btnExtrude;
    lv_obj_t* btnFan;
    lv_obj_t* btnTemperature;
    lv_obj_t* btnMacros;
    lv_obj_t* btnDisableMotors;
    lv_obj_t* btnConsole;
    
    // State
    float extruderTemp;
    float bedTemp;
    
    // Methods
    void createLayout();
    void createStatusbar();
    void createButtonGrid();
    void createActionButton(lv_obj_t* parent, const char* label, lv_color_t barColor, lv_coord_t col, lv_coord_t row);
    
    // Callbacks
    static void onMoveClick(lv_event_t* e);
    static void onExtrudeClick(lv_event_t* e);
    static void onFanClick(lv_event_t* e);
    static void onTemperatureClick(lv_event_t* e);
    static void onMacrosClick(lv_event_t* e);
    static void onDisableMotorsClick(lv_event_t* e);
    static void onConsoleClick(lv_event_t* e);
};

#endif // SCREEN_ACTIONS_H
