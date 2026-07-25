#ifndef SCREEN_HOME_V2_H
#define SCREEN_HOME_V2_H

#include "screen_base.h"

class ScreenHomeV2 : public ScreenBase {
public:
    ScreenHomeV2();
    void load() override;
    
private:
    // Layout containers
    lv_obj_t* leftPanel;
    lv_obj_t* rightPanel;
    lv_obj_t* tempGraph;
    
    // Temperature labels
    lv_obj_t* extruderTempLabel;
    lv_obj_t* bedTempLabel;
    lv_obj_t* piTempLabel;
    
    // Menu buttons
    lv_obj_t* btnHoming;
    lv_obj_t* btnTemperature;
    lv_obj_t* btnActions;
    lv_obj_t* btnConfiguration;
    lv_obj_t* btnPrint;
    
    // State
    float extruderTemp;
    float bedTemp;
    float piTemp;
    
    // Methods
    void createLayout();
    void createLeftPanel();
    void createTemperatureList();
    void createTemperatureGraph();
    void createRightPanel();
    void createMenuButtons();
    void createIconWithBar(lv_obj_t* parent, const char* icon, const char* label, 
                           lv_coord_t x, lv_coord_t y, lv_color_t barColor);
    
    // Callbacks
    static void onHomingClick(lv_event_t* e);
    static void onTemperatureClick(lv_event_t* e);
    static void onActionsClick(lv_event_t* e);
    static void onConfigurationClick(lv_event_t* e);
    static void onButtonClick(lv_event_t* e);
};

#endif // SCREEN_HOME_V2_H
