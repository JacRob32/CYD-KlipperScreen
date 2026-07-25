#ifndef SCREEN_TEMPERATURE_H
#define SCREEN_TEMPERATURE_H

#include "screen_base.h"

class ScreenTemperature : public ScreenBase {
public:
    ScreenTemperature();
    void load() override;
    
private:
    // Statusbar elements
    lv_obj_t* statusbar;
    lv_obj_t* lblExtruderTemp;
    lv_obj_t* lblBedTemp;
    lv_obj_t* lblMcuTemp;
    lv_obj_t* lblTitle;
    lv_obj_t* lblTime;
    
    // Left panel
    lv_obj_t* leftPanel;
    lv_obj_t* tempList;
    lv_obj_t* graphContainer;
    
    // Right panel
    lv_obj_t* rightPanel;
    lv_obj_t* presetGrid;
    
    // Temperature labels
    lv_obj_t* lblExtruderValue;
    lv_obj_t* lblBedValue;
    lv_obj_t* lblMcuValue;
    lv_obj_t* lblPiValue;
    
    // State
    float extruderTemp;
    float bedTemp;
    float mcuTemp;
    float piTemp;
    
    // Methods
    void createLayout();
    void createStatusbar();
    void createLeftPanel();
    void createTemperatureList();
    void createTempRow(lv_obj_t* parent, const char* icon, const char* label, float temp, 
                       lv_color_t barColor, lv_coord_t y);
    void createTemperatureGraph();
    void createRightPanel();
    void createPresetGrid();
    void createPresetButton(lv_obj_t* parent, const char* icon, const char* label, 
                            lv_color_t barColor, lv_coord_t col, lv_coord_t row);
    
    // Callbacks
    static void onCoolClick(lv_event_t* e);
    static void onPresetClick(lv_event_t* e);
};

#endif // SCREEN_TEMPERATURE_H
