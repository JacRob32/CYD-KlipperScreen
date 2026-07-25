#ifndef SCREEN_HOME_H
#define SCREEN_HOME_H
//screen_home.h
#include "screen_base.h"

class ScreenHome : public ScreenBase {
public:
    ScreenHome();
    void load() override;
    void update() override;
    
private:
    // UI Elements
    lv_obj_t* statusbar;
    lv_obj_t* tempContainer;
    lv_obj_t* menuButtons;
    
    // State
    bool isConnected;
    float bedTemp;
    float bedTarget;
    float extruderTemp;
    float extruderTarget;
    
    // Methods
    void createLayout();
    void createStatusbar();
    void createTemperatureDisplay();
    void createMenuGrid();
    
    // Callbacks
    static void onExtrudeClick(lv_event_t* e);
    static void onMoveClick(lv_event_t* e);
    static void onTempClick(lv_event_t* e);
    static void onSettingsClick(lv_event_t* e);
};

#endif // SCREEN_HOME_H
