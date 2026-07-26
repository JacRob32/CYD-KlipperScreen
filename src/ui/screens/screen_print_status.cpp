#include "ui/screens/screen_print_status.h"
#include "ui/ui_manager.h"
#include <Arduino.h>

extern UIManager* g_uiManager;

ScreenPrintStatus::ScreenPrintStatus()
    : extruderTemp(19.0)
    , bedTemp(20.0)
    , mcuTemp(22.0)
    , printProgress(5)
    , flowRate(0.0)
    , pressureAdvance(0.00)
    , filamentUsed(0.0)
    , filamentTotal(0.0)
    , isPrinting(true) {
    
    setScreenName("Print Status");
}

void ScreenPrintStatus::load() {
    Serial.println(F("[ScreenPrintStatus] Loading..."));
    
    // Create main screen object
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    
    // Use vertical flex layout
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createLayout();
    
    Serial.println(F("[ScreenPrintStatus] Loaded successfully"));
}

void ScreenPrintStatus::update() {
    // Update progress arc
    if (progressArc) {
        lv_arc_set_value(progressArc, printProgress);
    }
    
    // Update progress label
    if (progressLabel) {
        lv_label_set_text_fmt(progressLabel, "%d%%", printProgress);
    }
    
    // Update stats
    if (lblFlowrate) {
        lv_label_set_text_fmt(lblFlowrate, "%.1f mm³/s", flowRate);
    }
    if (lblFilamentUsed) {
        lv_label_set_text_fmt(lblFilamentUsed, "%.1f m", filamentUsed);
    }
}

void ScreenPrintStatus::createLayout() {
    createStatusbar();
    
    // Main content area (flex row)
    lv_obj_t* mainContent = lv_obj_create(screen);
    lv_obj_set_size(mainContent, DISPLAY_WIDTH, DISPLAY_HEIGHT - 120);
    lv_obj_set_style_bg_opa(mainContent, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(mainContent, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(mainContent, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    // Left section (progress + preview)
    lv_obj_t* leftSection = lv_obj_create(mainContent);
    lv_obj_set_size(leftSection, 120, DISPLAY_HEIGHT - 150);
    lv_obj_set_style_bg_opa(leftSection, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(leftSection, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(leftSection, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 
                         LV_FLEX_ALIGN_START);
    
    createProgressIndicator();
    
    // Center section (file info)
    lv_obj_t* centerSection = lv_obj_create(mainContent);
    lv_obj_set_size(centerSection, 120, DISPLAY_HEIGHT - 150);
    lv_obj_set_style_bg_opa(centerSection, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(centerSection, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(centerSection, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 
                         LV_FLEX_ALIGN_START);
    
    createFileInfo();
    
    // Right section (stats)
    lv_obj_t* rightSection = lv_obj_create(mainContent);
    lv_obj_set_size(rightSection, 120, DISPLAY_HEIGHT - 150);
    lv_obj_set_style_bg_opa(rightSection, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(rightSection, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(rightSection, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_all(rightSection, 5, LV_PART_MAIN);
    
    createPrintStats(rightSection);
    
    createBottomActions();
}

void ScreenPrintStatus::createStatusbar() {
    // Statusbar container
    statusbar = lv_obj_create(screen);
    lv_obj_set_size(statusbar, DISPLAY_WIDTH, 30);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(statusbar, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(statusbar, 0);
    
    // Back button (chevron)
    lv_obj_t* btnBack = lv_btn_create(statusbar);
    lv_obj_set_size(btnBack, 24, 20);
    lv_obj_align(btnBack, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_opa(btnBack, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btnBack, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
        Serial.println(F("[ScreenPrintStatus] Back button clicked"));
        if (g_uiManager) {
            g_uiManager->goBack();
        }
    }, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* backIcon = lv_label_create(btnBack);
    lv_label_set_text(backIcon, "◂");
    lv_obj_center(backIcon);
    lv_obj_set_style_text_font(backIcon, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(backIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Temperature indicators (left side)
    lv_obj_t* tempContainer = lv_obj_create(statusbar);
    lv_obj_set_size(tempContainer, 100, 20);
    lv_obj_set_pos(tempContainer, 35, 5);
    lv_obj_set_style_bg_opa(tempContainer, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(tempContainer, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(tempContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tempContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 
                         LV_FLEX_ALIGN_START);
    
    // Extruder temp
    lv_obj_t* icon1 = lv_label_create(tempContainer);
    lv_label_set_text(icon1, "");
    lv_obj_set_style_text_font(icon1, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon1, lv_color_hex(0xFF6B00), LV_PART_MAIN);
    
    lblExtruderTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblExtruderTemp, "%d°", (int)extruderTemp);
    lv_obj_set_style_text_font(lblExtruderTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblExtruderTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Spacer
    lv_obj_t* spacer1 = lv_obj_create(tempContainer);
    lv_obj_set_size(spacer1, 6, 1);
    lv_obj_set_style_bg_opa(spacer1, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Bed temp
    lv_obj_t* icon2 = lv_label_create(tempContainer);
    lv_label_set_text(icon2, "🔥");
    lv_obj_set_style_text_font(icon2, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon2, lv_color_hex(0x20B2AA), LV_PART_MAIN);
    
    lblBedTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblBedTemp, "%d°", (int)bedTemp);
    lv_obj_set_style_text_font(lblBedTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblBedTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Spacer
    lv_obj_t* spacer2 = lv_obj_create(tempContainer);
    lv_obj_set_size(spacer2, 6, 1);
    lv_obj_set_style_bg_opa(spacer2, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Mcu temp
    lv_obj_t* lblMcu = lv_label_create(tempContainer);
    lv_label_set_text(lblMcu, "M:");
    lv_obj_set_style_text_font(lblMcu, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblMcu, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    lblMcuTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblMcuTemp, "%d°", (int)mcuTemp);
    lv_obj_set_style_text_font(lblMcuTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblMcuTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Title (center)
    lblTitle = lv_label_create(statusbar);
    lv_label_set_text(lblTitle, "Mini | Print Status");
    lv_obj_align(lblTitle, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(lblTitle, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Time (right)
    lblTime = lv_label_create(statusbar);
    lv_label_set_text(lblTime, "20:45");
    lv_obj_align(lblTime, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(lblTime, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

void ScreenPrintStatus::createProgressIndicator() {
    // Progress container
    progressContainer = lv_obj_create(screen);
    lv_obj_set_size(progressContainer, 80, 80);
    lv_obj_set_pos(progressContainer, 20, 40);
    lv_obj_set_style_bg_opa(progressContainer, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Circular progress arc
    progressArc = lv_arc_create(progressContainer);
    lv_obj_set_size(progressArc, 70, 70);
    lv_obj_center(progressArc);
    lv_arc_set_range(progressArc, 0, 100);
    lv_arc_set_value(progressArc, printProgress);
    lv_arc_set_bg_angles(progressArc, 0, 360);
    lv_arc_set_start_angle(progressArc, 270);
    lv_arc_set_rotation(progressArc, 270);
    
    // Arc styles
    lv_obj_set_style_arc_color(progressArc, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_set_style_arc_width(progressArc, 6, LV_PART_MAIN);
    lv_obj_set_style_arc_color(progressArc, lv_color_hex(0xD32F2F), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(progressArc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(progressArc, true, LV_PART_INDICATOR);
    
    // Progress percentage label
    progressLabel = lv_label_create(progressContainer);
    lv_label_set_text_fmt(progressLabel, "%d%%", printProgress);
    lv_obj_center(progressLabel);
    lv_obj_set_style_text_font(progressLabel, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(progressLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

void ScreenPrintStatus::createFileInfo() {
    // Filename label
    lblFilename = lv_label_create(screen);
    lv_label_set_text(lblFilename, "Tests/No-extusion");
    lv_obj_set_pos(lblFilename, 110, 40);
    lv_obj_set_style_text_font(lblFilename, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblFilename, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Status label
    lblStatus = lv_label_create(screen);
    lv_label_set_text(lblStatus, "Printing");
    lv_obj_set_pos(lblStatus, 110, 65);
    lv_obj_set_style_text_font(lblStatus, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblStatus, lv_color_hex(0x888888), LV_PART_MAIN);
}

void ScreenPrintStatus::createPrintStats(lv_obj_t* parent) {
    // Stats container
    lv_obj_t* statsContainer = lv_obj_create(parent);
    lv_obj_set_size(statsContainer, 115, 140);
    lv_obj_set_pos(statsContainer, 0, 40);
    lv_obj_set_style_bg_opa(statsContainer, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Create stats rows
    createStatsRow(statsContainer, "Flow:", "100%", 0);
    createStatsRow(statsContainer, "Flowrate:", "0.0 mm³/s", 25);
    createStatsRow(statsContainer, "Pressure Advance:", "0.00", 50);
    createStatsRow(statsContainer, "Filament used:", "0.0 m", 75);
    createStatsRow(statsContainer, "Filament total:", "0.0 m", 100);
}

void ScreenPrintStatus::createStatsRow(lv_obj_t* parent, const char* label, const char* value,
                                        lv_coord_t y) {
    // Row container
    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_set_size(row, 115, 20);
    lv_obj_set_pos(row, 0, y);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
    
    // Label
    lv_obj_t* labelObj = lv_label_create(row);
    lv_label_set_text(labelObj, label);
    lv_obj_set_pos(labelObj, 0, 2);
    applyLabelStyle(labelObj, lv_color_hex(0xE0E0E0), 12);
    
    // Value (right aligned)
    lv_obj_t* valueObj = lv_label_create(row);
    lv_label_set_text(valueObj, value);
    lv_obj_align(valueObj, LV_ALIGN_RIGHT_MID, 0, 0);
    applyLabelStyle(valueObj, lv_color_hex(0xFFFFFF), 12);
    
    // Store pointers for updates
    if (strcmp(label, "Flowrate:") == 0) {
        lblFlowrate = valueObj;
    } else if (strcmp(label, "Filament used:") == 0) {
        lblFilamentUsed = valueObj;
    }
}

void ScreenPrintStatus::createBottomActions() {
    // Bottom action bar container
    lv_obj_t* actionBar = lv_obj_create(screen);
    lv_obj_set_size(actionBar, DISPLAY_WIDTH, 90);
    lv_obj_set_pos(actionBar, 0, DISPLAY_HEIGHT - 90);
    lv_obj_set_style_bg_color(actionBar, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(actionBar, 0, LV_PART_MAIN);
    
    createActionButtons(actionBar);
}

void ScreenPrintStatus::createActionButtons(lv_obj_t* parent) {
    // Create 4 action buttons
    createActionButton(parent, "", "Pause", lv_color_hex(0xFF6B00), 0);
    createActionButton(parent, "■", "Cancel", lv_color_hex(0xE91E63), 60);
    createActionButton(parent, "≡", "Fine Tuning", lv_color_hex(0x20B2AA), 120);
    createActionButton(parent, "⚙", "Settings", lv_color_hex(0xAEEA00), 180);
}

void ScreenPrintStatus::createActionButton(lv_obj_t* parent, const char* icon, const char* label,
                                             lv_color_t barColor, lv_coord_t x) {
    // Button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 55, 80);
    lv_obj_set_pos(btn, x, 5);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 4, LV_PART_MAIN);
    
    // Icon
    lv_obj_t* iconLabel = lv_label_create(btn);
    lv_label_set_text(iconLabel, icon);
    lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(iconLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Label
    lv_obj_t* textLabel = lv_label_create(btn);
    lv_label_set_text(textLabel, label);
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, 5);
    lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Colored bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 50, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback
    if (strcmp(label, "Pause") == 0) {
        lv_obj_add_event_cb(btn, onPauseClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Cancel") == 0) {
        lv_obj_add_event_cb(btn, onCancelClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Fine Tuning") == 0) {
        lv_obj_add_event_cb(btn, onFineTuningClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Settings") == 0) {
        lv_obj_add_event_cb(btn, onSettingsClick, LV_EVENT_CLICKED, NULL);
    }
}

void ScreenPrintStatus::onPauseClick(lv_event_t* e) {
    Serial.println(F("[ScreenPrintStatus] Pause clicked"));
    // TODO: Send M25 (pause print)
}

void ScreenPrintStatus::onCancelClick(lv_event_t* e) {
    Serial.println(F("[ScreenPrintStatus] Cancel clicked"));
    // TODO: Show confirmation dialog, then send M25
}

void ScreenPrintStatus::onFineTuningClick(lv_event_t* e) {
    Serial.println(F("[ScreenPrintStatus] Fine Tuning clicked"));
    // TODO: Navigate to fine tuning screen
}

void ScreenPrintStatus::onSettingsClick(lv_event_t* e) {
    Serial.println(F("[ScreenPrintStatus] Settings clicked"));
    // TODO: Navigate to print settings screen
}
