#ifndef SCREEN_EXTRUDE_H
#define SCREEN_EXTRUDE_H

#include "screen_base.h"

class ScreenExtrude : public ScreenBase {
public:
    ScreenExtrude();
    void load() override;
    
private:
    // Statusbar elements
    lv_obj_t* statusbar;
    lv_obj_t* lblExtruderTemp;
    lv_obj_t* lblBedTemp;
    lv_obj_t* lblTitle;
    lv_obj_t* lblTime;
    
    // Extruder selection
    lv_obj_t* extruderRow;
    lv_obj_t* btnT0;
    lv_obj_t* btnT1;
    lv_obj_t* btnT2;
    lv_obj_t* btnTemp;
    lv_obj_t* lblT0Temp;
    lv_obj_t* lblT1Temp;
    lv_obj_t* lblT2Temp;
    
    // Main action buttons
    lv_obj_t* actionRow;
    lv_obj_t* btnExtrude;
    lv_obj_t* btnLoad;
    lv_obj_t* btnUnload;
    lv_obj_t* btnRetract;
    
    // Distance buttons
    lv_obj_t* distRow;
    lv_obj_t* btnDist5;
    lv_obj_t* btnDist10;
    lv_obj_t* btnDist15;
    lv_obj_t* btnDist25;
    
    // Speed buttons
    lv_obj_t* speedRow;
    lv_obj_t* btnSpeed1;
    lv_obj_t* btnSpeed2;
    lv_obj_t* btnSpeed5;
    lv_obj_t* btnSpeed25;
    
    // State
    float extruderTemp;
    float bedTemp;
    int selectedExtruder;
    int selectedDistance;
    int selectedSpeed;
    
    // Methods
    void createLayout();
    void createStatusbar();
    void createExtruderRow();
    void createExtruderButton(lv_obj_t* parent, const char* label, const char* temp, lv_coord_t col, bool selected);
    void createTempButton(lv_obj_t* parent);
    void createActionRow();
    void createActionButton(lv_obj_t* parent, const char* icon, const char* label, lv_color_t barColor, lv_coord_t col);
    void createToggleRow(lv_obj_t* parent, const char* title, const char** options, int count, int selected, lv_color_t selectedColor);
    
    // Callbacks
    static void onExtruderClick(lv_event_t* e);
    static void onExtrudeClick(lv_event_t* e);
    static void onLoadClick(lv_event_t* e);
    static void onUnloadClick(lv_event_t* e);
    static void onRetractClick(lv_event_t* e);
    static void onDistClick(lv_event_t* e);
    static void onSpeedClick(lv_event_t* e);
};

#endif // SCREEN_EXTRUDE_H
