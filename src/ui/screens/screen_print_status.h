#ifndef SCREEN_PRINT_STATUS_H
#define SCREEN_PRINT_STATUS_H

#include "screen_base.h"

class ScreenPrintStatus : public ScreenBase {
public:
    ScreenPrintStatus();
    void load() override;
    void update() override;
    
private:
    // Statusbar elements
    lv_obj_t* statusbar;
    lv_obj_t* lblExtruderTemp;
    lv_obj_t* lblBedTemp;
    lv_obj_t* lblMcuTemp;
    lv_obj_t* lblTitle;
    lv_obj_t* lblTime;
    
    // Progress indicator
    lv_obj_t* progressContainer;
    lv_obj_t* progressArc;
    lv_obj_t* progressLabel;
    
    // File info
    lv_obj_t* lblFilename;
    lv_obj_t* lblStatus;
    
    // Stats labels
    lv_obj_t* lblFlow;
    lv_obj_t* lblFlowrate;
    lv_obj_t* lblPressureAdvance;
    lv_obj_t* lblFilamentUsed;
    lv_obj_t* lblFilamentTotal;
    
    // State
    float extruderTemp;
    float bedTemp;
    float mcuTemp;
    int printProgress;
    float flowRate;
    float pressureAdvance;
    float filamentUsed;
    float filamentTotal;
    bool isPrinting;
    
    // Methods
    void createLayout();
    void createStatusbar();
    void createProgressIndicator();
    void createFileInfo();
    void createPrintStats();
    void createStatsRow(lv_obj_t* parent, const char* label, const char* value, lv_coord_t y);
    void createBottomActions();
    void createActionButtons();
    void createActionButton(lv_obj_t* parent, const char* icon, const char* label, 
                            lv_color_t barColor, lv_coord_t x);
    
    // Callbacks
    static void onPauseClick(lv_event_t* e);
    static void onCancelClick(lv_event_t* e);
    static void onFineTuningClick(lv_event_t* e);
    static void onSettingsClick(lv_event_t* e);
};

#endif // SCREEN_PRINT_STATUS_H
